#include "sink/rolling_file_sink.h"

#include <stdint.h>

#include "core/context.h"
#include "util/environment.h"
#include "util/posix_error.h"
#include "util/writable_file.h"
#include "logbox/format.h"

namespace {

const uint32_t kBytesPerMegabytes = 1024 * 1024;

}

namespace logbox {

RollingFileSink::RollingFileSink(
    LogSeverity severity,
    const Slice& base_name)
    : Sink(severity),
    base_name_(Environment::GetBasename(base_name.data())),
    file_size_(0),
    bytes_since_flush_(0),
    next_flush_time_(0),
    rollover_attempts_(kRolloverBackoffAttempts - 1) {
}

std::unique_ptr<Sink> RollingFileSink::New(
    LogSeverity severity, const Slice& base_name) {
  return std::unique_ptr<Sink>(new RollingFileSink(severity, base_name));
}

void RollingFileSink::Send(const LoggingEvent& event) {
  if (event.severity() >= min_severity()) {
    LogToFile(event);
  }
}

void RollingFileSink::LogToFile(const LoggingEvent& event) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsRolloverNeeded()) {
    CloseLogFile();
  }

  if (!file_ && !AttemptOpenNewLogFile()) {
    return;
  }

  AppendToLogFile(event.message());
  MaybeFlush(event);
}

bool RollingFileSink::IsRolloverNeeded() {
  return file_size_ >= Context::Default()->max_log_size() * kBytesPerMegabytes;
}

void RollingFileSink::CloseLogFile() {
  file_.reset(nullptr);
  file_size_ = 0;
  rollover_attempts_ = kRolloverBackoffAttempts - 1;
  ResetFlushStatistics();
}

void RollingFileSink::ResetFlushStatistics() {
  bytes_since_flush_ = 0;
  next_flush_time_ = NextFlushTime();
}

Timestamp RollingFileSink::NextFlushTime() const {
  int64_t flush_interval = Context::Default()->flush_interval() *
      Timestamp::kMicroSecondsPerSeconds;
  return Environment::Now().AddMicroSeconds(flush_interval);
}

bool RollingFileSink::AttemptOpenNewLogFile() {
  if (++rollover_attempts_ != kRolloverBackoffAttempts) {
    return false;
  }

  rollover_attempts_ = 0;
  if (!OpenLogFile()) {
    return false;
  }

  return true;
}

const std::unique_ptr<WritableFile>& RollingFileSink::OpenLogFile() {
  const std::string& file_path = Environment::GetFilePath(
      Context::Default()->log_dir(), GetLogFileName());

  file_ = Environment::NewWritableFile(file_path);
  if (!file_) {
    char buff[100];
    int preserved_errno = errno;
    posix_strerror_r(preserved_errno, buff, sizeof(buff));
    fprintf(stderr, "WARNING: Could not create log file: %s[%d] - %s\n",
            buff, preserved_errno, file_path.c_str());
    return file_;
  }

  WriteLogFileHeader();
  return file_;
}

std::string RollingFileSink::GetLogFileName() const {
  // Log filename: <base name> <timestamp> <pid>
  return fmt::format("{}.{}.{}.log", base_name_,
                     Environment::Now().LocalFileNameString(),
                     Environment::GetProcessId());
}

void RollingFileSink::WriteLogFileHeader() {
  std::string header;
  AssembleLogFileHeader(&header);
  AppendToLogFile(header);
}

void RollingFileSink::AssembleLogFileHeader(std::string* header) const {
  fmt::MemoryWriter writer;
  writer << "Log file created at: " << Environment::Now().LocalString() << "\n";
  writer << "Log format: [DIWEF] yyyy-mm-dd hh:mm:ss.uuuuuu thread-id ";
  writer << "file:line func] msg\n";

  header->assign(writer.data(), writer.size());
}

void RollingFileSink::AppendToLogFile(const Slice& message) {
  file_->Append(message);
  file_size_ += message.size();
  bytes_since_flush_ += message.size();
}

void RollingFileSink::MaybeFlush(const LoggingEvent& event) {
  if (IsFlushNeeded(event)) {
    Flush();
  }
}

bool RollingFileSink::IsFlushNeeded(const LoggingEvent& event) const {
  return event.severity() > Context::Default()->max_buffered_severity() ||
      bytes_since_flush_ >= kMaxBufferedBytes ||
      next_flush_time_ < Environment::Now();
}

void RollingFileSink::Flush() {
  file_->Flush();
  ResetFlushStatistics();
}

}  // namespace logbox
