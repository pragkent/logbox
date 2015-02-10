#include <time.h>
#include <stdio.h>
#include <string>
#include "util/timestamp.h"

#include <gtest/gtest.h>

using logbox::Timestamp;

namespace {

Timestamp ts(1423494061302000L);

}

TEST(TimestampTest, Timestamp) {
  EXPECT_EQ(1423494061302000L, ts.timestamp());
}

TEST(TimestampTest, SecondsAndMicroSeconds) {
  EXPECT_EQ(1423494061L, ts.seconds());
  EXPECT_EQ(302000L, ts.microseconds());
}

TEST(TimestampTest, AddMicroSeconds) {
  Timestamp time_after = ts.AddMicroSeconds(10000);
  EXPECT_EQ(1423494061312000L, time_after.timestamp());
}

TEST(TimestampTest, LocalString) {
  struct tm tm_time;
  time_t t = ts.seconds();
  localtime_r(&t, &tm_time);

  char time_buffer[100] = {0};
  strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &tm_time);

  char time_buffer_with_us[100] = {0};
  snprintf(time_buffer_with_us, sizeof(time_buffer_with_us), "%s.%d",
           time_buffer, ts.microseconds());

  EXPECT_EQ(time_buffer_with_us, ts.LocalString());
}

TEST(TimestampTest, UTCString) {
  EXPECT_EQ("2015-02-09 15:01:01.302000", ts.UtcString());
}

TEST(TimestampTest, LocalFileNameString) {
  struct tm tm_time;
  time_t t = ts.seconds();
  localtime_r(&t, &tm_time);

  char time_buffer[100] = {0};
  strftime(time_buffer, sizeof(time_buffer), "%Y%m%d-%H%M%S", &tm_time);
  EXPECT_EQ(time_buffer, ts.LocalFileNameString());
}

TEST(TimestampTest, UtcFileNameString) {
  EXPECT_EQ("20150209-150101", ts.UtcFileNameString());
}

TEST(TimestampTest, LessThanOperator) {
  EXPECT_LT(Timestamp(-1), Timestamp(0));
  EXPECT_LT(Timestamp(0), Timestamp(1));
  EXPECT_FALSE(Timestamp(0) < Timestamp(-1));
  EXPECT_FALSE(Timestamp(0) < Timestamp(0));
}

TEST(TimestampTest, EqualOperator) {
  EXPECT_EQ(Timestamp(-1), Timestamp(-1));
  EXPECT_EQ(Timestamp(1), Timestamp(1));
  EXPECT_FALSE(Timestamp(0) == Timestamp(1));
}
