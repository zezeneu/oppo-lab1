#include "date_time.h"

#include <cstdio>
#include <stdexcept>

Date MakeDate(int year, int month, int day) {
  const Date date{std::chrono::year{year},
                  std::chrono::month{static_cast<unsigned>(month)},
                  std::chrono::day{static_cast<unsigned>(day)}};
  if (!date.ok() || year < 1) {
    throw std::runtime_error("Такой даты нет в календаре: " +
                             std::to_string(year) + "." +
                             std::to_string(month) + "." +
                             std::to_string(day));
  }
  return date;
}

Time MakeTime(int hours, int minutes) {
  if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
    throw std::runtime_error("Недопустимое время (нужно 00:00 – 23:59)");
  }
  return Time{hours, minutes};
}

std::string FormatDate(const Date& date) {
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%04d.%02u.%02u",
                static_cast<int>(date.year()), unsigned{date.month()},
                unsigned{date.day()});
  return buffer;
}

std::string FormatTime(const Time& time) {
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%02d:%02d", time.hours, time.minutes);
  return buffer;
}
