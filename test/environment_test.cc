#include <string>
#include "util/environment.h"

#include <gtest/gtest.h>

using logbox::Environment;

TEST(EnvironmentTest, GetBasename) {
  EXPECT_STREQ("", Environment::GetBasename(""));
  EXPECT_STREQ("bar", Environment::GetBasename("bar"));
  EXPECT_STREQ("bar", Environment::GetBasename("./foo/bar"));
  EXPECT_STREQ("bar.txt", Environment::GetBasename("./foo/bar.txt"));
  EXPECT_STREQ("bar.txt", Environment::GetBasename("/foo/bar.txt"));
}

TEST(EnvironmentTest, GetFilePath) {
  EXPECT_EQ("", Environment::GetFilePath("", ""));
  EXPECT_EQ("bar.txt", Environment::GetFilePath("", "bar.txt"));
  EXPECT_EQ("./foo/bar.txt", Environment::GetFilePath("./foo", "bar.txt"));
  EXPECT_EQ("/foo/bar.txt", Environment::GetFilePath("/foo", "bar.txt"));
}
