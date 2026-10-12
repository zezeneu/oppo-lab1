/// @file
/// @brief Время суток с точностью до минуты (00:00 – 23:59).
#ifndef LESSONS_TIME_OF_DAY_H_
#define LESSONS_TIME_OF_DAY_H_

#include <string>
#include <tuple>

class TimeOfDay {
 public:
  static constexpr int kMaxHours = 23;
  static constexpr int kMaxMinutes = 59;

  /// Бросает RangeError, если часы не в 0..23 или минуты не в 0..59.
  TimeOfDay(int hours, int minutes);

  int hours() const { return hours_; }
  int minutes() const { return minutes_; }

  /// Возвращает время в виде "чч:мм" с ведущими нулями.
  std::string ToString() const;

  friend bool operator==(const TimeOfDay& a, const TimeOfDay& b) {
    return a.Key() == b.Key();
  }
  friend bool operator!=(const TimeOfDay& a, const TimeOfDay& b) {
    return !(a == b);
  }
  friend bool operator<(const TimeOfDay& a, const TimeOfDay& b) {
    return a.Key() < b.Key();
  }

 private:
  std::tuple<int, int> Key() const { return std::make_tuple(hours_, minutes_); }

  int hours_;
  int minutes_;
};

#endif  // LESSONS_TIME_OF_DAY_H_
