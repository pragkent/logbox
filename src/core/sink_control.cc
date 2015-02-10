#include "core/sink_control.h"

#include <stdio.h>

namespace {

std::once_flag flag;
logbox::SinkControl* default_control;

const char* kFallbackWarning = "WARNING: Logging before InitializeLogging() "
    "is written to STDERR\n";
bool fallback_warned = false;

}

namespace logbox {

SinkControl* SinkControl::Default() {
  std::call_once(flag, [](){ default_control = new logbox::SinkControl; });
  return default_control;
}

bool SinkControl::HasSink() const {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  return !sinks_.empty();
}

void SinkControl::AttachSink(std::unique_ptr<Sink>&& sink) {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  sinks_.push_back(std::move(sink));
}

void SinkControl::Clear() {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  sinks_.clear();
}

void SinkControl::SendToSinks(const LoggingEvent& event) {
  std::vector<std::shared_ptr<Sink>> sinks;

  {
    // We only hold lock during copy attached sinks. Once copied,
    // we release lock.
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    sinks = sinks_;

    // If we don't have any sink attached, send it to stderr.
    if (sinks_.empty()) {
      SendToStderr(event);
      return;
    }
  }

  for (auto& sink : sinks) {
    sink->Send(event);
  }
}

// Since this function is called with lock been hold, we
// don't need another lock for it.
void SinkControl::SendToStderr(const LoggingEvent& event) {
  if (!fallback_warned) {
    fputs(kFallbackWarning, stderr);
    fallback_warned = true;
  }

  fwrite(event.message().data(), event.message().size(), 1, stderr);
}

} // namespace logbox
