// Copyright (c) 1999, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "util/posix_error.h"

#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "logbox/feature_toggle.h"

namespace logbox {

int posix_strerror_r(int err, char *buf, size_t len) {
  // Sanity check input parameters
  if (buf == NULL || len <= 0) {
    errno = EINVAL;
    return -1;
  }

  // Reset buf and errno, and try calling whatever version of strerror_r()
  // is implemented by glibc
  buf[0] = '\0';
  int old_errno = errno;
  errno = 0;
  char *rc = reinterpret_cast<char *>(strerror_r(err, buf, len));

  // Both versions set errno on failure
  if (errno) {
    // Should already be there, but better safe than sorry
    buf[0] = '\0';
    return -1;
  }
  errno = old_errno;

  // POSIX is vague about whether the string will be terminated, although
  // is indirectly implies that typically ERANGE will be returned, instead
  // of truncating the string. This is different from the GNU implementation.
  // We play it safe by always terminating the string explicitly.
  buf[len - 1] = '\0';

  // If the function succeeded, we can use its exit code to determine the
  // semantics implemented by glibc
  if (!rc) {
    return 0;
  } else {
    // GNU semantics detected
    if (rc == buf) {
      return 0;
    } else {
      buf[0] = '\0';
#if defined(OS_MACOSX) || defined(OS_FREEBSD) || defined(OS_OPENBSD)
      if (reinterpret_cast<intptr_t>(rc) < sys_nerr) {
        // This means an error on MacOSX or FreeBSD.
        return -1;
      }
#endif
      strncat(buf, rc, len-1);
      return 0;
    }
  }
}

std::string StrError(int err) {
  char buff[100];
  int rc = posix_strerror_r(err, buff, sizeof(buff));
  if (rc < 0 || buff[0] == '\0') {
    snprintf(buff, sizeof(buff), "Error number %d", err);
  }

  return buff;
}

}
