#include "util/posix_error.h"

#include <errno.h>
#include <gtest/gtest.h>

using logbox::posix_strerror_r;

TEST(PosixStrErrorTest, InvalidBuffer) {
  EXPECT_EQ(-1, posix_strerror_r(1, NULL, 0));
}

TEST(PosixStrErrorTest, StrError) {
  char buff[100];
  EXPECT_EQ(0, posix_strerror_r(EINVAL, buff, sizeof(buff)));
  EXPECT_STREQ("Invalid argument", buff);
}
