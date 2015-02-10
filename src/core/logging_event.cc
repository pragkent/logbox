#include "core/logging_event.h"

#include <errno.h>
#include "util/environment.h"

namespace logbox {

LoggingEvent::LoggingEvent(
    LogSeverity severity,
    const char* file,
    int line,
    const char* function)
    : severity_(severity),
      file_fullname_(file),
      file_basename_(Environment::GetBasename(file)),
      line_no_(line),
      function_(function),
      preserved_errno_(errno),
      timestamp_(Environment::Now()) {
}

}  // namespace logbox
