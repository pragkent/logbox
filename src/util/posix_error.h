#ifndef LOGBOX_UTIL_POSIX_ERROR_H_
#define LOGBOX_UTIL_POSIX_ERROR_H_

#include <stddef.h>

namespace logbox {

// Portable thread-safe version of strerror. It returns 0 and stores
// error string in user-supplied buffer on success.
int posix_strerror_r(int err, char *buf, size_t len);

} // namespace logbox

#endif // LOGBOX_UTIL_POSIX_ERROR_H_
