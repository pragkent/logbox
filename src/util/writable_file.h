#ifndef LOGBOX_UTIL_WRITABLE_FILE_H_
#define LOGBOX_UTIL_WRITABLE_FILE_H_

#include <stddef.h>
#include <stdio.h>
#include <string>

#include "util/slice.h"

namespace logbox {

// WritableFile represents a file object which could be
// appended.
class WritableFile {
 public:

  // Create a writable file with given filename and FILE object.
  WritableFile(const std::string& filename, FILE* file);

  // Destructor would close FILE object if it's not closed.
  ~WritableFile();

  const std::string& filename() const { return filename_; }

  size_t size() const { return size_; }

  bool Append(const Slice& slice);
  bool Close();
  bool Flush();

 private:
  std::string filename_;
  size_t size_;
  FILE* file_;
};

}  // namespace logbox

#endif  // LOGBOX_UTIL_WRITABLE_FILE_H_
