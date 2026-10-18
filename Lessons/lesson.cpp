#include "lesson.h"

#include <regex>
#include <stdexcept>

namespace {

// Строка целиком: дата, время, имя в кавычках. Склеено из трёх частей:
//   дата  [0-9]{4}\.[0-9]{1,2}\.[0-9]{1,2}   (месяц и день – 1 или 2 цифры)
//   время [0-9]{1,2}:[0-9]{1,2}               (часы и минуты – 1 или 2 цифры)
//   имя   "[^"]+"                              (любые символы, кроме кавычки)
// Скобки запоминают куски: год, месяц, день, часы, минуты, имя.
const std::regex& LinePattern() {
  static const std::regex kPattern(
      R"re(\s*([0-9]{4})\.([0-9]{1,2})\.([0-9]{1,2})\s+([0-9]{1,2}):([0-9]{1,2})\s+"([^"]+)"\s*)re");
  return kPattern;
}

}  // namespace

Lesson ParseLesson(const std::string& line) {
  std::smatch parts;
  if (!std::regex_match(line, parts, LinePattern())) {
    throw std::runtime_error(
        "Ожидалась строка вида гггг.мм.дд чч:мм \"Имя преподавателя\"");
  }
  const Date date = MakeDate(std::stoi(parts[1]), std::stoi(parts[2]),
                             std::stoi(parts[3]));
  const Time time = MakeTime(std::stoi(parts[4]), std::stoi(parts[5]));
  return Lesson{date, time, parts[6]};
}
