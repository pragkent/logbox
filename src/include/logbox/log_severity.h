#ifndef LOGBOX_LOG_SEVERITY_H_
#define LOGBOX_LOG_SEVERITY_H_

namespace logbox {

enum LogSeverity {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  FATAL = 4,
  NUM_SEVERITIES,
};

// Returns the name of given severity.
const char* NameOfSeverity(LogSeverity severity);

} // namespace logbox

#endif // LOGBOX_LOG_SEVERITY_H_
