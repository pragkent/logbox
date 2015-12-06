#include "util/posix_error.h"

#include <errno.h>
#include <gtest/gtest.h>

using logbox::StrError;

TEST(PosixStrErrorTest, StrError) {
  EXPECT_EQ("Invalid argument", StrError(EINVAL));
}
