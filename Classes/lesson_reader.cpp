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

// Читает следующую строку. Возвращает false в конце ввода и на пустой строке.
// В первой строке отбрасывает метку BOM, которую добавляют некоторые редакторы.
bool NextLine(std::istream& in, bool is_first, std::string* line) {
  if (!std::getline(in, *line)) {
    return false;
  }
  if (is_first) {
    StripUtf8Bom(line);
  }
  return !Trim(*line).empty();
}

}  // namespace

ReadResult LessonReader::ReadAll(std::istream& in) {
  ReadResult result;
  std::string line;
  size_t number = 0;
  while (NextLine(in, number == 0, &line)) {
    ParseLine(line, ++number, &result);
  }
  return result;
}
