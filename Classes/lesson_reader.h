/// @file
/// @brief Чтение занятий из потока с сообщениями об ошибочных строках.
#ifndef LESSONS_LESSON_READER_H_
#define LESSONS_LESSON_READER_H_

#include <istream>
#include <string>
#include <vector>

#include "lesson_list.h"

/// Ошибка в одной строке ввода.
struct LineError {
  size_t line_number;  // номер строки, начиная с 1
  std::string message;
};

/// Результат чтения: корректные занятия и ошибки по остальным строкам.
struct ReadResult {
  LessonList lessons{};
  std::vector<LineError> errors{};
};

class LessonReader {
 public:
  /// Читает строки до пустой строки (или конца ввода). Ошибочная строка не
  /// прерывает чтение: она попадает в errors, остальные обрабатываются.
  static ReadResult ReadAll(std::istream& in);
};

#endif  // LESSONS_LESSON_READER_H_
