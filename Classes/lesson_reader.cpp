#include "lesson_reader.h"

#include "errors.h"
#include "parsing_utils.h"

namespace {

// Разбирает одну строку. Ожидаемые ошибки (LessonError) записываются в
// результат; любые другие исключения – признак бага и не перехватываются.
void ParseLine(const std::string& line, size_t number, ReadResult* result) {
  try {
    result->lessons.Add(Lesson::Parse(line));
  } catch (const LessonError& error) {
    result->errors.push_back(LineError{number, error.what()});
  }
}

}  // namespace

ReadResult LessonReader::ReadAll(std::istream& in) const {
  ReadResult result;
  std::string line;
  size_t number = 0;
  while (std::getline(in, line) && !Trim(line).empty()) {
    ParseLine(line, ++number, &result);
  }
  return result;
}
