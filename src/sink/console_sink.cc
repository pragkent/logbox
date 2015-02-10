#include "sink/console_sink.h"

#include <cassert>
#include <stdio.h>

namespace logbox {

std::unique_ptr<Sink> ConsoleSink::New(LogSeverity severity) {
  return std::unique_ptr<Sink>(new ConsoleSink(severity));
}

void ConsoleSink::Send(const LoggingEvent& event) {
  if (event.severity() >= min_severity()) {
    fwrite(event.message().data(), event.message().size(), 1, stderr);
  }
}

} // namespace logbox
