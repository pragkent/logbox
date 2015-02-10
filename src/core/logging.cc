#include "logbox/logging.h"

#include <errno.h>
#include <stdlib.h>
#include <memory>

#include "logbox/log_severity.h"

#include "core/context.h"
#include "core/sink_control.h"
#include "sink/console_sink.h"
#include "sink/rolling_file_sink.h"
#include "util/environment.h"
#include "util/posix_error.h"
#include "util/slice.h"
#include "util/timestamp.h"

namespace logbox {

class LogBox::LogBoxImpl {
 public:
  LogBoxImpl(
      LogSeverity severity,
      const char* filename,
      int line_no,
      const char* function)
      : event_(severity, filename, line_no, function) {
    WriteLogPrefix();
  }

  ~LogBoxImpl() {
    Archive();
    if (event_.severity() == FATAL) {
      Fail();
    }
  }

  LogStream& stream() { return stream_; }

  int preserved_errno() const {
    return event_.preserved_errno();
  }

 private:
  void Archive();

  void WriteLogPrefix();

  void SealLogMessage() {
    stream_ << "\n";
    event_.set_message(Slice(stream_.data(), stream_.size()));
  }

  void LogToSinks() {
    SinkControl::Default()->SendToSinks(event_);
  }

  void RestoreErrno() {
    errno = event_.preserved_errno();
  }

  void Fail() ATTRIBUTE_NORETURN;

  LoggingEvent event_;
  LogStream stream_;
};

void LogBox::LogBoxImpl::WriteLogPrefix() {
  stream_.Write(
      "{} {} {:05} {}:{} {}] ",
      NameOfSeverity(event_.severity())[0],
      event_.timestamp().LocalString(),
      Environment::GetThreadId(),
      event_.file_basename(),
      event_.line_no(),
      event_.function());
}

void LogBox::LogBoxImpl::Archive() {
  SealLogMessage();
  LogToSinks();
  RestoreErrno();
}

void LogBox::LogBoxImpl::Fail() {
  abort();
}

LogBox::LogBox(const char* file, int line, const char* function)
    : impl_(new LogBoxImpl(logbox::INFO, file, line, function)) {
    }

LogBox::LogBox(
    const char* file,
    int line,
    const char* function,
    LogSeverity severity)
    : impl_(new LogBoxImpl(severity, file, line, function)) {
    }


LogBox::LogBox(
    const char* file,
    int line,
    const char* function,
    LogSeverity severity,
    const char* message)
    : impl_(new LogBoxImpl(severity, file, line, function)) {
    impl_->stream() << message;
}

LogBox::~LogBox() {}

LogStream& LogBox::stream() { return impl_->stream(); }

int LogBox::preserved_errno() const {
  return impl_->preserved_errno();
}

ErrnoLogBox::ErrnoLogBox(
    const char* file,
    int line,
    const char* function,
    LogSeverity severity)
    : LogBox(file, line, function, severity) {
}

ErrnoLogBox::~ErrnoLogBox() {
  char buf[100];
  posix_strerror_r(preserved_errno(), buf, sizeof(buf));
  stream() << ": " << buf << " [" << preserved_errno() << "]";
}

bool IsLoggingInitialized() {
  return SinkControl::Default()->HasSink();
}

void SetLogDir(const std::string& log_dir) {
  CHECK(!IsLoggingInitialized()) << __func__ << " must be called before "
      "InitializeLogging";

  Context::Default()->set_log_dir(log_dir);
}

void SetMaxLogSize(uint32_t max_size) {
  CHECK(!IsLoggingInitialized()) << __func__ << " must be called before "
      "InitializeLogging";

  Context::Default()->set_max_log_size(max_size);
}

void SetFlushInterval(uint32_t interval) {
  CHECK(!IsLoggingInitialized()) << __func__ << " must be called before "
      "InitializeLogging";

  Context::Default()->set_flush_interval(interval);
}

void SetMaxBufferSeverity(uint32_t severity) {
  CHECK(!IsLoggingInitialized()) << __func__ << " must be called before "
      "InitializeLogging";

  Context::Default()->set_max_buffered_severity(severity);
}

void InitializeLogging(LogSeverity min_severity) {
  CHECK(!IsLoggingInitialized()) << __func__ << " should be called only once";

  SinkControl::Default()->AttachSink(ConsoleSink::New(min_severity));
}

void InitializeLogging(LogSeverity min_severity, const char* log_base_name) {
  CHECK(!IsLoggingInitialized()) << __func__ << " should be called only once";

  SinkControl::Default()->AttachSink(
      RollingFileSink::New(min_severity, log_base_name));
}

}  // namespace logbox
