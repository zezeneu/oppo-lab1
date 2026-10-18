#include "lesson.h"

#include <regex>

#include "errors.h"
#include "parsing_utils.h"

namespace {

// Вся строка целиком: гггг.мм.дд чч:мм "Имя". Существование даты и диапазон
// времени регулярное выражение не проверяет – это делают Date и TimeOfDay.
// Склеено из трёх частей:
//   дата  [0-9]{4}\.[0-9]{1,2}\.[0-9]{1,2}  (месяц и день – 1 или 2 цифры)
//   время [0-9]{1,2}:[0-9]{1,2}              (часы и минуты – 1 или 2 цифры)
//   имя   "[^"]+"                             (любые символы, кроме кавычки)
// Скобки запоминают куски: год, месяц, день, часы, минуты, имя.
const std::regex& LinePattern() {
  static const std::regex kPattern(
      R"re(\s*([0-9]{4})\.([0-9]{1,2})\.([0-9]{1,2})\s+([0-9]{1,2}):([0-9]{1,2})\s+"([^"]+)"\s*)re");
  return kPattern;
}

// std::regex разбирает строку рекурсивно: на очень длинных строках стек
// переполняется, поэтому длину ограничиваем заранее.
constexpr size_t kMaxLineLength = 1000;

// Номера групп регулярного выражения.
constexpr size_t kYearGroup = 1;
constexpr size_t kMonthGroup = 2;
constexpr size_t kDayGroup = 3;
constexpr size_t kHoursGroup = 4;
constexpr size_t kMinutesGroup = 5;
constexpr size_t kNameGroup = 6;

const char* const kFormatHint =
    "Ожидалась строка вида гггг.мм.дд чч:мм \"Имя преподавателя\": ";

int ToInt(const std::ssub_match& part) { return std::stoi(part.str()); }

// Разбирает строку регулярным выражением; при несовпадении бросает SyntaxError.
// Результат ссылается на line, поэтому line должна жить дольше результата.
std::smatch MatchLine(const std::string& line) {
  std::smatch parts;
  if (line.size() > kMaxLineLength ||
      !std::regex_match(line, parts, LinePattern())) {
    throw SyntaxError(kFormatHint + line.substr(0, kMaxLineLength));
  }
  return parts;
}

}  // namespace

Lesson::Lesson(Date date, TimeOfDay time, const std::string& teacher_name)
    : date_(date), time_(time), teacher_name_(Trim(teacher_name)) {
  if (teacher_name_.empty()) {
    throw RangeError("Имя преподавателя не может быть пустым");
  }
}

Lesson Lesson::Parse(const std::string& line) {
  const std::smatch parts = MatchLine(line);
  const Date lesson_date(ToInt(parts[kYearGroup]), ToInt(parts[kMonthGroup]),
                         ToInt(parts[kDayGroup]));
  const TimeOfDay lesson_time(ToInt(parts[kHoursGroup]),
                              ToInt(parts[kMinutesGroup]));
  return {lesson_date, lesson_time, parts[kNameGroup].str()};
}

bool Lesson::StartsBefore(const Lesson& other) const {
  if (date_ != other.date_) {
    return date_ < other.date_;
  }
  return time_ < other.time_;
}

std::string Lesson::ToString() const {
  return date_.ToString() + "  " + time_.ToString() + "  " + teacher_name_;
}
