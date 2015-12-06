#ifndef LOGBOX_UTIL_POSIX_ERROR_H_
#define LOGBOX_UTIL_POSIX_ERROR_H_

#include <stddef.h>
#include <string>

namespace logbox {

// Portable thread-safe version of strerror. It returns a string
// describing the given POSIX error num.
std::string StrError(int err);

} // namespace logbox

#endif // LOGBOX_UTIL_POSIX_ERROR_H_
