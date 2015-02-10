#include "core/context.h"
#include "logbox/log_severity.h"

#include <mutex>

namespace {

std::once_flag flag;
logbox::Context* default_context;

const char* kDefaultLogDir = ".";

const uint32_t kDefaultMaxLogSize = 300; // 300MB

const uint32_t kDefaultFlushInterval = 10;

const uint32_t kDefaultMaxBufferedSeverity = logbox::INFO;

}

namespace logbox {

Context::Context()
    : log_dir_(kDefaultLogDir),
      max_log_size_(kDefaultMaxLogSize),
      flush_interval_(kDefaultFlushInterval),
      max_buffered_severity_(kDefaultMaxBufferedSeverity) {
}

Context* Context::Default() {
  std::call_once(flag, []() { default_context = new Context; });
  return default_context;
}

}
