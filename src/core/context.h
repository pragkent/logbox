#ifndef LOGBOX_CORE_CONTEXT_H_
#define LOGBOX_CORE_CONTEXT_H_

#include <stdint.h>
#include <string>

namespace logbox {

class Context {
 public:
  Context();
  static Context* Default();

  const std::string& log_dir() const {
    return log_dir_;
  }

  void set_log_dir(const std::string& log_dir) {
    log_dir_ = log_dir;
  }

  uint32_t max_log_size() const {
    return max_log_size_;
  }

  void set_max_log_size(uint32_t max_size) {
    max_log_size_ = max_size;
  }

  uint32_t flush_interval() const {
    return flush_interval_;
  }

  void set_flush_interval(uint32_t interval) {
    flush_interval_ = interval;
  }

  uint32_t max_buffered_severity() const {
    return max_buffered_severity_;
  }

  void set_max_buffered_severity(uint32_t severity) {
    max_buffered_severity_ = severity;
  }

 private:
  std::string log_dir_;
  uint32_t max_log_size_;
  uint32_t flush_interval_;
  uint32_t max_buffered_severity_;
};

} // namespace logbox

#endif // LOGBOX_CORE_CONTEXT_H_
