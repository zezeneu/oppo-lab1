/// @file
/// @brief Календарная дата. Объект всегда содержит существующую дату.
#ifndef LESSONS_DATE_H_
#define LESSONS_DATE_H_

#include <chrono>
#include <string>

class Date {
 public:
  static constexpr int kMinYear = 1;
  static constexpr int kMaxYear = 9999;

  /// Бросает RangeError, если такой даты не существует (например, 30 февраля).
  /// Существование даты проверяет std::chrono::year_month_day::ok().
  Date(int year, int month, int day);

  int year() const { return static_cast<int>(value_.year()); }
  int month() const { return static_cast<int>(unsigned{value_.month()}); }
  int day() const { return static_cast<int>(unsigned{value_.day()}); }

  /// Возвращает дату в виде "гггг.мм.дд" с ведущими нулями.
  std::string ToString() const;

  friend bool operator==(const Date& a, const Date& b) {
    return a.value_ == b.value_;
  }
  friend bool operator!=(const Date& a, const Date& b) { return !(a == b); }
  friend bool operator<(const Date& a, const Date& b) {
    return a.value_ < b.value_;
  }

 private:
  std::chrono::year_month_day value_;
};

#endif  // LESSONS_DATE_H_
