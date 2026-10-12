#include <sstream>

#include "gtest_lite.h"

#include "application.h"

namespace {

struct Run {
  int code;
  std::string out;
  std::string err;
};

Run RunWithInput(const std::string& input) {
  std::istringstream in(input);
  std::ostringstream out;
  std::ostringstream err;
  Application application(in, out, err);
  int code = application.Run();
  return Run{code, out.str(), err.str()};
}

bool Contains(const std::string& text, const std::string& part) {
  return text.find(part) != std::string::npos;
}

}  // namespace

TEST(Application, FullScenarioProducesExpectedOutput) {
  Run run = RunWithInput(
      "2024.03.11 14:30 \"Иванов И.И.\"\n"
      "2024.03.11 09:00 \"Петров П.П.\"\n"
      "2024.03.11 11:15 \"Иванов И.И.\"\n"
      "\n"
      "Иванов И.И.\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_EQ(run.err, "");
  EXPECT_EQ(
      run.out,
      "Вводите занятия по одному в строке:\n"
      "гггг.мм.дд чч:мм \"Имя преподавателя\"\n"
      "Пустая строка завершает ввод.\n"
      "\nВсе занятия (в порядке ввода):\n"
      "2024.03.11  14:30  Иванов И.И.\n"
      "2024.03.11  09:00  Петров П.П.\n"
      "2024.03.11  11:15  Иванов И.И.\n"
      "\nЗанятия по возрастанию времени:\n"
      "2024.03.11  09:00  Петров П.П.\n"
      "2024.03.11  11:15  Иванов И.И.\n"
      "2024.03.11  14:30  Иванов И.И.\n"
      "\nВведите имя преподавателя для фильтра: "
      "\nЗанятия преподавателя \"Иванов И.И.\":\n"
      "2024.03.11  11:15  Иванов И.И.\n"
      "2024.03.11  14:30  Иванов И.И.\n");
}

TEST(Application, BadLinesGoToErrorStreamAndAreSkipped) {
  Run run = RunWithInput(
      "2024.03.11 10:00 \"А\"\nмусор\n2024.02.30 10:00 \"Б\"\n\nА\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.err, "Строка 2 пропущена"));
  EXPECT_TRUE(Contains(run.err, "Строка 3 пропущена"));
  EXPECT_FALSE(Contains(run.out, "пропущена"));
  EXPECT_TRUE(Contains(run.out, "2024.03.11  10:00  А"));
}

TEST(Application, EmptyInputPrintsPlaceholders) {
  Run run = RunWithInput("");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.out, "(нет занятий)"));
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"\":"));
}

TEST(Application, MissingFilterLineMeansEmptyFilter) {
  Run run = RunWithInput("2024.03.11 10:00 \"А\"\n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"\":\n(нет занятий)"));
}

TEST(Application, UnknownTeacherGivesNoLessons) {
  Run run = RunWithInput("2024.03.11 10:00 \"А\"\n\nБ\n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"Б\":\n(нет занятий)"));
}

TEST(Application, FilterNameIsTrimmed) {
  Run run = RunWithInput("2024.03.11 10:00 \"А\"\n\n   А  \n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"А\":\n2024.03.11"));
}

TEST(Application, SortingUsesDateBeforeTime) {
  Run run = RunWithInput(
      "2024.03.12 08:00 \"А\"\n2024.03.11 18:00 \"Б\"\n\nА\n");
  size_t first = run.out.find("по возрастанию");
  ASSERT_TRUE(first != std::string::npos);
  size_t b = run.out.find("18:00  Б", first);
  size_t a = run.out.find("08:00  А", first);
  EXPECT_TRUE(b < a);
}

TEST(Application, OnlyErrorsStillCompletes) {
  Run run = RunWithInput("x\ny\n\nА\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.err, "Строка 1 пропущена"));
  EXPECT_TRUE(Contains(run.out, "(нет занятий)"));
}

TEST(Application, InputFileWithBomKeepsFirstLesson) {
  Run run = RunWithInput("\xEF\xBB\xBF" "2024.03.11 09:00 \"A\"\n\nA\n");
  EXPECT_TRUE(run.err.empty());
  EXPECT_TRUE(Contains(run.out, "2024.03.11  09:00  A"));
}
