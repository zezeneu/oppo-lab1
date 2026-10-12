#include <sstream>

#include "gtest_lite.h"

#include "lesson_printer.h"

namespace {

Lesson MakeLesson(int hours, const char* teacher) {
  return Lesson(Date(2024, 3, 5), TimeOfDay(hours, 5), teacher);
}

}  // namespace

TEST(LessonPrinter, PrintsTitleAndLessons) {
  std::ostringstream out;
  LessonList list;
  list.Add(MakeLesson(8, "Иванов"));
  list.Add(MakeLesson(9, "Петров"));
  LessonPrinter(out).PrintSection("Заголовок:", list);
  EXPECT_EQ(out.str(),
            "\nЗаголовок:\n2024.03.05  08:05  Иванов\n2024.03.05  09:05  Петров\n");
}

TEST(LessonPrinter, PrintsPlaceholderForEmptyList) {
  std::ostringstream out;
  LessonPrinter(out).PrintSection("Пусто:", LessonList());
  EXPECT_EQ(out.str(), "\nПусто:\n(нет занятий)\n");
}

TEST(LessonPrinter, PrintsNothingForNoErrors) {
  std::ostringstream out;
  LessonPrinter(out).PrintErrors({});
  EXPECT_EQ(out.str(), "");
}

TEST(LessonPrinter, PrintsOneLinePerError) {
  std::ostringstream out;
  LessonPrinter(out).PrintErrors({LineError{2, "плохо"}, LineError{5, "ещё хуже"}});
  EXPECT_EQ(out.str(),
            "Строка 2 пропущена: плохо\nСтрока 5 пропущена: ещё хуже\n");
}
