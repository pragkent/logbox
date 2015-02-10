#ifndef LOGBOX_UTIL_TIMESTAMP_H_
#define LOGBOX_UTIL_TIMESTAMP_H_

#include <stdint.h>
#include <time.h>
#include <string>

namespace logbox {

class Timestamp {
 public:
  // Create a Timestamp from microseconds elapsed since Epoch.
  explicit Timestamp(int64_t timestamp) : timestamp_(timestamp) {
  }

  // Default copy constrcutor and assignment operator are fine.
  Timestamp(const Timestamp& other) = default;
  Timestamp& operator=(const Timestamp& other) = default;

  // Return microseconds have elapsed since Epoch.
  int64_t timestamp() {
    return timestamp_;
  }

  // Gets seconds component of the time represented by this instance.
  time_t seconds() const {
    return static_cast<time_t>(timestamp_ / kMicroSecondsPerSeconds);
  }

  // Gets microseconds component of the time represented by this instance.
  int microseconds() const {
    return static_cast<int>(timestamp_ % kMicroSecondsPerSeconds);
  }

  // Returns a new Timestamp that adds the specified number of microseconds
  // to the value of this instance.
  Timestamp AddMicroSeconds(int64_t duration_in_us) {
    return Timestamp(timestamp_ + duration_in_us);
  }

  // Converts the time represented by this instance to its equaivalent
  // string representation in local time. It's been used in log line.
  // Format: yyyy-mm-dd hh:mm:ss.uuuuuu
  // Example: 2015-02-09 23:55:03.622782
  std::string LocalString() const;

  // Converts the time represented by this instance to its equaivalent
  // string representation in local time. It's been used in log file name.
  // Format: yyyymmdd-hhmmss
  // Example: 20150209-235503
  std::string LocalFileNameString() const;

  // Similar to LocalString except that we use UTC time here.
  std::string UtcString() const;

  // Similar to LocalFileNameString except that we use UTC time here.
  std::string UtcFileNameString() const;

  static const int kMicroSecondsPerSeconds = 1000 * 1000;

 private:
  // BreakTime point to a function which convert time_t to struct tm.
  // localtime_r and gmtime_r are two of them.
  typedef struct tm* (*BreakTime)(const time_t*, struct tm*);

  std::string ToString(BreakTime break_time) const;
  std::string ToFileNameString(BreakTime break_time) const;

  // Number of microsends since Epoch.
  int64_t timestamp_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.timestamp() < rhs.timestamp();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.timestamp() == rhs.timestamp();
}

} // namespace logbox

#endif // LOGBOX_UTIL_TIMESTAMP_H_
