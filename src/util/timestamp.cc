#include "util/timestamp.h"

#include <stdio.h>
#include <time.h>

namespace logbox {

std::string Timestamp::LocalString() const {
  return ToString(localtime_r);
}

// Convert timestamp to string representation. break_time is
// used to break time_t into struct tm.
std::string Timestamp::ToString(BreakTime break_time) const {
  struct tm tm_time;
  time_t sec = seconds();
  break_time(&sec, &tm_time);

  char buff[32] = {0};
  snprintf(buff, sizeof(buff), "%4d-%02d-%02d %02d:%02d:%02d.%06d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds());
  return buff;
}

std::string Timestamp::UtcString() const {
  return ToString(gmtime_r);
}

std::string Timestamp::LocalFileNameString() const {
  return ToFileNameString(localtime_r);
}
  
// Convert timestamp to string representation been used in filename.
// break_time is used to break time_t into struct tm.
std::string Timestamp::ToFileNameString(BreakTime break_time) const {
  struct tm tm_time;
  time_t sec = seconds();
  break_time(&sec, &tm_time);

  char buff[32] = {0};
  snprintf(buff, sizeof(buff), "%4d%02d%02d-%02d%02d%02d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  return buff;
}

std::string Timestamp::UtcFileNameString() const {
  return ToFileNameString(gmtime_r);
}

}
