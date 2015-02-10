#ifndef LOGBOX_SINK_ROLLING_FILE_SINK_H_
#define LOGBOX_SINK_ROLLING_FILE_SINK_H_

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <mutex>

#include "core/logging_event.h"
#include "core/sink.h"
#include "util/timestamp.h"
#include "util/slice.h"

namespace logbox {

class WritableFile;
class RollingFileSink : public Sink {
 public:
  static std::unique_ptr<Sink> New(
      LogSeverity severity, const Slice& base_name);

  virtual void Send(const LoggingEvent& event) override;

 private:
  RollingFileSink(
      LogSeverity severity,
      const Slice& base_name);

  void LogToFile(const LoggingEvent& event);

  bool IsRolloverNeeded();
  void CloseLogFile();
  void ResetFlushStatistics();
  Timestamp NextFlushTime() const;
  bool AttemptOpenNewLogFile();
  const std::unique_ptr<WritableFile>& OpenLogFile();
  std::string GetLogFileName() const;
  void WriteLogFileHeader();
  void AssembleLogFileHeader(std::string* header) const;
  void AppendToLogFile(const Slice& message);

  void MaybeFlush(const LoggingEvent& event);
  bool IsFlushNeeded(const LoggingEvent& event) const;
  void Flush();

  static const uint16_t kRolloverBackoffAttempts = 32;
  static const uint32_t kMaxBufferedBytes = 1024 * 1024;

  std::mutex mutex_;
  std::unique_ptr<WritableFile> file_;
  std::string base_name_;
  size_t file_size_;
  size_t bytes_since_flush_;
  Timestamp next_flush_time_;
  uint16_t rollover_attempts_;
};

} // namespace logbox

#endif // LOGBOX_SINK_ROLLING_FILE_SINK_H_
