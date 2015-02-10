#ifndef LOGBOX_CORE_SINK_CONTROL_H_
#define LOGBOX_CORE_SINK_CONTROL_H_

#include <memory>
#include <mutex>
#include <vector>

#include "logbox/log_severity.h"
#include "core/logging_event.h"
#include "core/sink.h"

namespace logbox {

// SinkControl manages all the active sinks. It sends logging
// event to all the sinks attached.
class SinkControl {
 public:
  SinkControl() {}

  // Disallow copy constructor and copy assignment.
  SinkControl(const SinkControl& other) = delete;
  SinkControl& operator=(const SinkControl& other) = delete;

  // Returns a default SinkControl with no sink attached.
  // The returned object should not be deleted.
  static SinkControl* Default();

  // Is there any sink attached to this control?
  bool HasSink() const;

  // Attach a new sink to this control. Ownership of sink is
  // transfered to SinkControl.
  void AttachSink(std::unique_ptr<Sink>&& sink);

  // Remove all the attached sinks.
  void Clear();

  // Send logging event to all attached sinks.
  void SendToSinks(const LoggingEvent& event);

 private:
  // If there is no sink attached, logging event would be
  // sent to stderr with a warning.
  void SendToStderr(const LoggingEvent& event);

  // Stores attached sinks.
  std::vector<std::shared_ptr<Sink>> sinks_;

  // Mutex to keep thread safe with sinks.
  mutable std::mutex sinks_mutex_;
};

}  // namespace logbox

#endif  // LOGBOX_CORE_SINK_CONTROL_H_
