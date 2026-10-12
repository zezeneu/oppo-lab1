#include "lesson.h"

#include <regex>
#include <stdexcept>

namespace {

// Строка целиком: гггг.мм.дд чч:мм "Имя преподавателя".
const std::regex& LinePattern() {
  static const std::regex kPattern(
      R"re(\s*(\d{4})\.(\d{2})\.(\d{2})\s+(\d{2}):(\d{2})\s+"([^"]*)"\s*)re");
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
