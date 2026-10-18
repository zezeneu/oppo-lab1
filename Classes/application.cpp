#include "application.h"

#include <string>
#include <utility>

#include "lesson_rules.h"
#include "parsing_utils.h"

// Порядок потоков зафиксирован документацией и проверяется тестами.
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Application::Application(std::istream& lessons_in, std::istream& console_in,
                         std::ostream& out, std::ostream& err)
    : lessons_in_(lessons_in),
      console_in_(console_in),
      out_(out),
      printer_(out),
      error_printer_(err) {}

int Application::Run() {
  LessonList lessons = ReadLessons();
  printer_.PrintSection("Все занятия (в порядке ввода):", lessons);
  lessons.SortBy(ByStartTime);
  printer_.PrintSection("Занятия по возрастанию времени:", lessons);
  ShowFiltered(lessons);
  return 0;
}

LessonList Application::ReadLessons() {
  ReadResult result = LessonReader::ReadAll(lessons_in_);
  error_printer_.PrintErrors(result.errors);
  return std::move(result.lessons);
}

void Application::ShowFiltered(const LessonList& lessons) {
  out_ << "\nВведите имя преподавателя для фильтра: ";
  std::string name;
  std::getline(console_in_, name);
  name = Trim(name);
  printer_.PrintSection("Занятия преподавателя \"" + name + "\":",
                        lessons.Filter(ByTeacher(name)));
}
