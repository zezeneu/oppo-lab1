#include "date.h"

#include <cstdio>

#include "errors.h"
#include "parsing_utils.h"

namespace {

constexpr int kMonthsInYear = 12;
constexpr int kMaxDaysInMonth = 31;

// chrono обрезает слишком большие числа, поэтому диапазоны проверяем заранее.
std::chrono::year_month_day MakeValue(int year, int month, int day) {
  CheckRange(year, Date::kMinYear, Date::kMaxYear, "Год");
  CheckRange(month, 1, kMonthsInYear, "Месяц");
  CheckRange(day, 1, kMaxDaysInMonth, "День");
  return {std::chrono::year{year},
          std::chrono::month{static_cast<unsigned>(month)},
          std::chrono::day{static_cast<unsigned>(day)}};
}

}  // namespace

Date::Date(int year, int month, int day) : value_(MakeValue(year, month, day)) {
  if (!value_.ok()) {
    throw RangeError("Такой даты нет в календаре: " + ToString());
  }
}

std::string Date::ToString() const {
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d", year(), month(),
                day());
  return buffer;
}
