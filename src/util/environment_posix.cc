#include "util/environment.h"

#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/time.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "logbox/feature_toggle.h"

#ifndef __NR_gettid
#ifdef SYS_gettid
#define __NR_gettid SYS_gettid
#elif !defined(__i386__)
#error "Must define __NR_gettid for non-x86 platforms"
#else
#define __NR_gettid 224
#endif
#endif

namespace {

const char PATH_SEPARATOR = '/';

pid_t gettid() {
  // On Linux and MacOSX, we try to use gettid().
  static bool lacks_gettid = false;
  if (!lacks_gettid) {
    pid_t tid = syscall(__NR_gettid);
    if (tid != -1) {
      return tid;
    }
    // Technically, this variable has to be volatile, but there is a small
    // performance penalty in accessing volatile variables and there should
    // not be any serious adverse effect if a thread does not immediately see
    // the value change to "true".
    lacks_gettid = true;
  }

  // If none of the techniques above worked, we use pthread_self().
  return static_cast<pid_t>(reinterpret_cast<uintptr_t>(pthread_self()));
}
}

namespace logbox {

pid_t Environment::GetProcessId() { return getpid(); }

pid_t Environment::GetThreadId() { return gettid(); }

Timestamp Environment::Now() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);

  int64_t useconds = tv.tv_sec * Timestamp::kMicroSecondsPerSeconds + tv.tv_usec;
  return Timestamp(useconds);
}

std::string Environment::GetFilePath(
    const std::string& dir, const std::string& filename) {
  std::string path = dir;
  if (path.empty()) {
    return filename;
  }

  if (path.back() != PATH_SEPARATOR) {
    path.push_back(PATH_SEPARATOR);
  }

  path += filename;
  return path;
}

const char* Environment::GetBasename(const char* filepath) {
  const char* base = strrchr(filepath, PATH_SEPARATOR);
  return base ? (base + 1) : filepath;
}

std::unique_ptr<WritableFile> Environment::NewWritableFile(
    const std::string& filename) {
  int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0664);
  if (fd == -1) {
    return std::unique_ptr<WritableFile>();
  }

  FILE* file = fdopen(fd, "a");

  // Cleanup mess if dfopen failed
  if (file == NULL) {
    close(fd);
    unlink(filename.c_str());
    return std::unique_ptr<WritableFile>();
  }

  return std::unique_ptr<WritableFile>(new WritableFile(filename, file));
}

}  // namespace logbox
