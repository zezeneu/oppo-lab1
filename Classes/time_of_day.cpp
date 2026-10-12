#include "time_of_day.h"

#include <cstdio>

#include "errors.h"
#include "parsing_utils.h"

TimeOfDay::TimeOfDay(int hours, int minutes)
    : hours_(hours), minutes_(minutes) {
  CheckRange(hours, 0, kMaxHours, "Часы");
  CheckRange(minutes, 0, kMaxMinutes, "Минуты");
}

std::string TimeOfDay::ToString() const {
  char buffer[16];
  std::snprintf(buffer, sizeof(buffer), "%02d:%02d", hours_, minutes_);
  return buffer;
}
