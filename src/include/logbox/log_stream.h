#ifndef LOGBOX_LOG_STREAM_H_
#define LOGBOX_LOG_STREAM_H_

#include "logbox/format.h"

namespace logbox {

class LogStream : public fmt::MemoryWriter {
 public:
  void Write(fmt::StringRef format, fmt::ArgList args) {
    write(format, args);
  }
  
  void Printf(fmt::StringRef format, fmt::ArgList args) {
    fmt::printf(*this, format, args);
  }

  LOGBOX_FMT_VARIADIC(void, Write, fmt::StringRef)
  LOGBOX_FMT_VARIADIC(void, Printf, fmt::StringRef)
};

}  // namespace logbox

#endif  // LOGBOX_LOG_STREAM_H_
