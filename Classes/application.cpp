#include "application.h"

#include <string>
#include <utility>

#include "lesson_rules.h"
#include "parsing_utils.h"

namespace {

const char* const kPrompt =
    "Вводите занятия по одному в строке:\n"
    "гггг.мм.дд чч:мм \"Имя преподавателя\"\n"
    "Пустая строка завершает ввод.\n";

}  // namespace

// Порядок потоков зафиксирован документацией и проверяется тестами.
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Application::Application(std::istream& in, std::ostream& out,
                         std::ostream& err)
    : in_(in), out_(out), printer_(out), error_printer_(err) {}

int Application::Run() {
  out_ << kPrompt;
  LessonList lessons = ReadLessons();
  printer_.PrintSection("Все занятия (в порядке ввода):", lessons);
  lessons.SortBy(ByStartTime);
  printer_.PrintSection("Занятия по возрастанию времени:", lessons);
  ShowFiltered(lessons);
  return 0;
}

LessonList Application::ReadLessons() {
  ReadResult result = LessonReader::ReadAll(in_);
  error_printer_.PrintErrors(result.errors);
  return std::move(result.lessons);
}

void Application::ShowFiltered(const LessonList& lessons) {
  out_ << "\nВведите имя преподавателя для фильтра: ";
  std::string name;
  std::getline(in_, name);
  name = Trim(name);
  printer_.PrintSection("Занятия преподавателя \"" + name + "\":",
                        lessons.Filter(ByTeacher(name)));
}
