/// @file
/// @brief Типы исключений для ошибок разбора и проверки занятий.
#ifndef LESSONS_ERRORS_H_
#define LESSONS_ERRORS_H_

#include <stdexcept>

/// Базовый класс всех ошибок, связанных с описанием занятия. Позволяет
/// перехватить любую «ожидаемую» ошибку одним catch.
class LessonError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

/// Нарушен формат строки: нет поля, нет кавычки, в числе не цифры и т. п.
class SyntaxError : public LessonError {
 public:
  using LessonError::LessonError;
};

/// Формат верный, но значение недопустимо: месяц 13, 30 февраля, пустое имя.
class RangeError : public LessonError {
 public:
  using LessonError::LessonError;
};

#endif  // LESSONS_ERRORS_H_
