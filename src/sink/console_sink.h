#ifndef LOGBOX_SINK_CONSOLE_SINK_H_
#define LOGBOX_SINK_CONSOLE_SINK_H_

#include <memory>

#include "core/logging_event.h"
#include "core/sink.h"

namespace logbox {

class ConsoleSink : public Sink {
 public:
  static std::unique_ptr<Sink> New(LogSeverity severity);

  virtual void Send(const LoggingEvent& event) override;

 private:
  explicit ConsoleSink(LogSeverity severity) : Sink(severity) {}
};

} // namespace logbox

#endif // LOGBOX_SINK_CONSOLE_SINK_H_
