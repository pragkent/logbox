#ifndef LOGBOX_UTIL_ENVIRONMENT_H_
#define LOGBOX_UTIL_ENVIRONMENT_H_

#include <sys/types.h>
#include <string>
#include <memory>

#include "util/timestamp.h"
#include "util/writable_file.h"

namespace logbox {

class Environment {
 public:
  // Returns current process ID.
  static pid_t GetProcessId();

  // Returns current thread ID. Native thread ID is returned if available,
  // pthread_self is returned otherwise.
  static pid_t GetThreadId();

  // Returns a Timestamp represents current time.
  static Timestamp Now();

  // Returns basename of filepath.
  static const char* GetBasename(const char* filepath);

  // Returns a filepath combines dir and filename.
  static std::string GetFilePath(
      const std::string& dir, const std::string& filename);

  // Create an object that writes to a new file with the specified
  // name. If file could not be opened, an empty unique_ptr is returned.
  //
  // The returned file will only be accessed by one thread at a time.
  static std::unique_ptr<WritableFile> NewWritableFile(
      const std::string& filename);
};

} // namespace logbox

#endif // LOGBOX_UTIL_ENVIRONMENT_H_
