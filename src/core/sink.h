#ifndef LOGBOX_CORE_SINK_H_
#define LOGBOX_CORE_SINK_H_

#include <stdint.h>

#include "logbox/log_severity.h"
#include "core/logging_event.h"

namespace logbox {

// Sink is a base class which represents the destination
// of logging.
class Sink {
 public:
  // Construct a Sink with specified min severity.
  explicit Sink(LogSeverity severity)
      : min_severity_(severity) {}

  // Disallow copy constructor and copy assignment operator.
  Sink(const Sink& other) = delete;
  Sink& operator=(const Sink& other) = delete;

  // Send logging event to this sink. It should be implemented
  // by all kinds of sinks.
  virtual void Send(const LoggingEvent& event) = 0;

  // Returns minimum log severity of this sink.
  LogSeverity min_severity() const { return min_severity_; }

 private:
  // Minimum log severity of this sink.
  LogSeverity min_severity_;
};

}  // namespace logbox

#endif  // LOGBOX_CORE_SINK_H_
