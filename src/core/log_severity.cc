#include "logbox/log_severity.h"

namespace {

const char* kSeverityNames[logbox::NUM_SEVERITIES] = {
  "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

logbox::LogSeverity NormalizeSeverity(logbox::LogSeverity severity) {
  if (severity < logbox::DEBUG) {
    return logbox::DEBUG;
  }

  if (severity > logbox::FATAL) {
    return logbox::ERROR;
  }

  return severity;
}

}

namespace logbox {

const char* NameOfSeverity(LogSeverity severity) {
  return kSeverityNames[NormalizeSeverity(severity)];
}

}
