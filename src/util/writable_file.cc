#include "util/writable_file.h"

#include <cassert>

namespace logbox {

WritableFile::WritableFile(const std::string& filename, FILE* file)
    : filename_(filename), file_(file) {
  assert(file_ != nullptr);
}


WritableFile:: ~WritableFile() {
  if (file_ != nullptr) {
    Close();
  }
}

bool WritableFile::Append(const Slice& slice) {
  assert(file_ != nullptr);

  return fwrite(slice.data(), slice.size(), 1, file_) != slice.size();
}

bool WritableFile::Close() {
  assert(file_ != nullptr);

  int code = fclose(file_);
  file_ = nullptr;
  return code != 0;
}

bool WritableFile::Flush() {
  assert(file_ != nullptr);

  return fflush(file_) != 0;
}

}  // namespace logbox
