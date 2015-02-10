#ifndef LOGBOX_LOGGING_H_
#define LOGBOX_LOGGING_H_

#include <stdint.h>
#include <memory>

#include "logbox/feature_toggle.h"
#include "logbox/log_severity.h"
#include "logbox/log_stream.h"

// A few macros for logging. Logging could be used in a few ways:
//
// Stream Style:
//   LOG(DEBUG) << "This is a cool debug log";
//
// Formatting Style:
//   LOG(INFO).Write("We find {} cookies", num_of_cookies);
//
// Printf Style
//   LOG(ERROR).Printf("We can use printf! %s", "Yeah!");
//
// You can do conditional logging this way:
//   LOG_IF(DEBUG, num > 10) << "Too many cookies to eat!";
//   LOG_IF(INFO, num > 10).Write("We got {} cookies!", num);
#define LOGBOX_LOG(severity) \
  logbox::LogBox(__FILE__, __LINE__, __func__, logbox::severity)

#define LOG(severity) LOGBOX_LOG(severity).stream()

#define LOG_IF(severity, condition)      \
  !(condition) ? static_cast<void>(0) \
               : logbox::LogBoxVoidify() & LOG(severity)

// CHECK dies with a FATAL log if condition is not true.
#define CHECK(condition)                                        \
  LOG_IF(FATAL, LOGBOX_PREDICT_BRANCH_NOT_TAKEN(!(condition))) \
      << "Check failed: " #condition " "

// CHECK_NOT_NULL could be handy in constructor initialize list.
#define CHECK_NOT_NULL(val)                          \
  logbox::CheckNotNull(__FILE__, __LINE__, __func__, \
                       "" #val "' Must be non NULL", (val))

// Logging with posix error message, like perror.
#define PLOG(severity) LOGBOX_PLOG(severity).stream()

#define LOGBOX_PLOG(severity)  \
  logbox::ErrnoLogBox( \
      __FILE__, __LINE__, __func__, logbox::severity)

#define PLOG_IF(severity, condition) \
  !(condition) ? (void)0 : logbox::LogBoxVoidify() & PLOG(severity)

#define PCHECK(condition)                                       \
  PLOG_IF(FATAL, LOGBOX_PREDICT_BRANCH_NOT_TAKEN(!(condition))) \
      << "Check failed: " #condition " "

#define CHECK_ERR(invocation)                                         \
  PLOG_IF(FATAL, LOGBOX_PREDICT_BRANCH_NOT_TAKEN((invocation) == -1)) \
      << #invocation

namespace logbox {

class LogBox {
 private:
  class LogBoxImpl;

 public:
  LogBox(const char* file, int line, const char* function);
  LogBox(
      const char* file,
      int line,
      const char* function,
      LogSeverity severity);
  LogBox(const char* file,
         int line,
         const char* function,
         LogSeverity severity,
         const char* message);
  ~LogBox();

  LogBox(const LogBox& other) = delete;
  LogBox& operator=(const LogBox& other) = delete;

  LogStream& stream();
  int preserved_errno() const;

 private:
  std::unique_ptr<LogBoxImpl> impl_;
};

class ErrnoLogBox : public LogBox {
 public:
  ErrnoLogBox(
      const char* file,
      int line,
      const char* function,
      LogSeverity severity);
  ErrnoLogBox(const ErrnoLogBox& other) = delete;
  ErrnoLogBox& operator=(const ErrnoLogBox& other) = delete;
  ~ErrnoLogBox();
};

// Only existed for LOG_IF and PLOG_IF.
class LogBoxVoidify {
 public:
  template <typename T>
  void operator&(T&) {}
};

template <typename T>
T* CheckNotNull(const char* file, int line, const char* func,
                const char* message, T* p) {
  if (p == nullptr) {
    LogBox(file, line, func, FATAL, message);
  }

  return p;
}

// Sets log directory where log files would be created.
// Default is current working directory.
void SetLogDir(const std::string& log_dir);

// Sets maximum log file size in megabytes.
// Default is 300M.
void SetMaxLogSize(uint32_t max_size);

// Sets maximum time in seconds between two flush operation.
// In other words, it means maximum time that logs could be buffered.
// Default is 10 seconds.
void SetFlushInterval(uint32_t interval);

// Sets maximum severity that might be buffered before write to
// file. Logs with higher severity would be flushed to file directly.
void SetMaxBufferSeverity(uint32_t severity);

// Initialize logging with given min_severity. Logs with higher
// severity will be sent to stderr.
void InitializeLogging(LogSeverity min_severity);

// Initialize logging with given min_severity and log_base_name.
// Logs with higher severity will be sent to file in log dir.
void InitializeLogging(LogSeverity min_severity, const char* log_base_name);

}  // namespace logbox

#endif  // LOGBOX_LOGGING_H_
