#include "time_of_day.h"

#include "errors.h"
#include "parsing_utils.h"

namespace {

constexpr size_t kPartWidth = 2;

}  // namespace

TimeOfDay::TimeOfDay(int hours, int minutes)
    : hours_(hours), minutes_(minutes) {
  CheckRange(hours, 0, kMaxHours, "Часы");
  CheckRange(minutes, 0, kMaxMinutes, "Минуты");
}

std::string TimeOfDay::ToString() const {
  return ZeroPad(hours_, kPartWidth) + ":" + ZeroPad(minutes_, kPartWidth);
}
