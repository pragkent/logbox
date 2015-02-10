#ifndef LOGBOX_CORE_LOGGING_EVENT_H_
#define LOGBOX_CORE_LOGGING_EVENT_H_

#include <stdint.h>
#include "logbox/log_severity.h"
#include "util/slice.h"
#include "util/timestamp.h"

namespace logbox {

class LoggingEvent {
 public:
  // Creates a new LoggingEvent with given severity, filename,
  // line_no and function. Besides, we preserve errno value.
  LoggingEvent(
      LogSeverity severity,
      const char* file,
      int line,
      const char* function);

  // Although it's safe to let LoggingEvent copyable, it doesn't
  // make any sense. Copy a LoggingEvent does not mean two events
  // happened. So we just disallow copy behavior.
  LoggingEvent(const LoggingEvent& other) = delete;
  LoggingEvent& operator=(const LoggingEvent& other) = delete;

  LogSeverity severity() const {
    return severity_;
  }

  const char* file_fullname() const {
    return file_fullname_;
  }

  const char* file_basename() const {
    return file_basename_;
  }

  int line_no() const {
    return line_no_;
  }

  const char* function() const {
    return function_;
  }

  int preserved_errno() const {
    return preserved_errno_;
  }

  Timestamp timestamp() const {
    return timestamp_;
  }

  const Slice& message() const {
    return message_;
  }

  void set_message(const Slice& msg) {
    message_ = msg;
  }

 private:
  LogSeverity severity_;
  const char* file_fullname_;
  const char* file_basename_;
  int line_no_;
  const char* function_;
  int preserved_errno_;
  Timestamp timestamp_;
  Slice message_;
};

}  // namespace logbox

#endif  // LOGBOX_CORE_LOGGING_EVENT_H_
