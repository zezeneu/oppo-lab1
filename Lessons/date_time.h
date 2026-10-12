// Дата и время занятия: типы, создание с проверкой и форматирование.
#ifndef LESSONS_DATE_TIME_H_
#define LESSONS_DATE_TIME_H_

#include <chrono>
#include <string>

// Календарная дата из стандартной библиотеки (C++20).
using Date = std::chrono::year_month_day;

struct Time {
  int hours;
  int minutes;
};

// Создаёт дату. Если такой даты нет в календаре (например, 30 февраля),
// бросает std::runtime_error. Проверку выполняет year_month_day::ok().
Date MakeDate(int year, int month, int day);

// Создаёт время. Если часы не в 0..23 или минуты не в 0..59,
// бросает std::runtime_error.
Time MakeTime(int hours, int minutes);

// Возвращает дату в виде "гггг.мм.дд" (с ведущими нулями).
std::string FormatDate(const Date& date);

// Возвращает время в виде "чч:мм" (с ведущими нулями).
std::string FormatTime(const Time& time);

#endif  // LESSONS_DATE_TIME_H_
