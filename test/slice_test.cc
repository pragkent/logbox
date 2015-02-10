#include <string>

#include "util/slice.h"

#include <gtest/gtest.h>

using logbox::Slice;

TEST(SliceTest, CreateByCharPointerAndLength) {
  const char* hello = "Hello";
  Slice slice(hello, 2);
  EXPECT_EQ(hello, slice.data());
  EXPECT_EQ(2U, slice.size());
}

TEST(SliceTest, CreateByStdString) {
  std::string hello = "Hello";
  Slice slice(hello);
  EXPECT_EQ(hello.data(), slice.data());
  EXPECT_EQ(hello.size(), slice.size());
}

TEST(SliceTest, CreateByCString) {
  const char* hello = "Hello";
  Slice slice(hello);
  EXPECT_EQ(hello, slice.data());
  EXPECT_EQ(5U, slice.size());
}

TEST(SliceTest, Empty) {
  EXPECT_TRUE(Slice("").empty());
  EXPECT_FALSE(Slice("Not empty").empty());
}

TEST(SliceTest, SubscriptOperator) {
  Slice slice("hello");
  EXPECT_EQ('h', slice[0]);
  EXPECT_EQ('e', slice[1]);
  EXPECT_EQ('l', slice[2]);
  EXPECT_EQ('l', slice[3]);
  EXPECT_EQ('o', slice[4]);
}

TEST(SliceTest, ToString) {
  Slice slice("hello", 2);
  EXPECT_EQ("he", slice.ToString());
}

TEST(SliceTest, Compare) {
  Slice hello("hello");
  Slice hi("hi");

  EXPECT_GT(0, hello.compare(hi));
  EXPECT_LT(0, hi.compare(hello));
  EXPECT_EQ(0, hi.compare(hi));
}

TEST(SliceTest, StartsWith) {
  Slice hello("hello");
  Slice hi("hi");
  Slice he("he");

  EXPECT_TRUE(hello.starts_with(he));
  EXPECT_TRUE(hello.starts_with(hello));
  EXPECT_FALSE(hello.starts_with(hi));
  EXPECT_FALSE(he.starts_with(hello));
}

TEST(Slice, EqualOperator) {
  Slice hello("Hello");
  Slice hi("Hi");

  EXPECT_TRUE(hello == hello);
  EXPECT_TRUE(hi == hi);
  EXPECT_FALSE(hello == hi);
}

TEST(Slice, NotEqualOperator) {
  Slice hello("Hello");
  Slice hi("Hi");

  EXPECT_TRUE(hello != hi);
  EXPECT_FALSE(hello != hello);
  EXPECT_FALSE(hi != hi);
}
