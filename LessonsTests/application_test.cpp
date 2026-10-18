#include <sstream>

#include <gtest/gtest.h>

#include "application.h"

namespace {

struct ProgramResult {
  int code;
  std::string out;
  std::string err;
};

// lessons – содержимое файла с занятиями, name – то, что вводят в консоли.
ProgramResult RunWithInput(const std::string& lessons,
                           const std::string& name = "") {
  std::istringstream file(lessons);
  std::istringstream console(name);
  std::ostringstream out;
  std::ostringstream err;
  Application application(file, console, out, err);
  int code = application.Run();
  return ProgramResult{code, out.str(), err.str()};
}

bool Contains(const std::string& text, const std::string& part) {
  return text.find(part) != std::string::npos;
}

}  // namespace

TEST(Application, FullScenarioProducesExpectedOutput) {
  ProgramResult run = RunWithInput(
      "2024.03.11 14:30 \"Иванов И.И.\"\n"
      "2024.03.11 09:00 \"Петров П.П.\"\n"
      "2024.03.11 11:15 \"Иванов И.И.\"\n",
      "Иванов И.И.\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_EQ(run.err, "");
  EXPECT_EQ(
      run.out,
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
  ProgramResult run = RunWithInput(
      "2024.03.11 10:00 \"А\"\nмусор\n2024.02.30 10:00 \"Б\"\n", "А\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.err, "Строка 2 пропущена"));
  EXPECT_TRUE(Contains(run.err, "Строка 3 пропущена"));
  EXPECT_FALSE(Contains(run.out, "пропущена"));
  EXPECT_TRUE(Contains(run.out, "2024.03.11  10:00  А"));
}

TEST(Application, BlankLinesInFileAreSkippedAndReadingContinues) {
  ProgramResult run = RunWithInput(
      "2024.03.11 10:00 \"А\"\n\n   \n2024.03.12 11:00 \"Б\"\n", "Б\n");
  EXPECT_TRUE(run.err.empty());
  EXPECT_TRUE(Contains(run.out, "2024.03.11  10:00  А"));
  EXPECT_TRUE(Contains(run.out, "2024.03.12  11:00  Б"));
}

TEST(Application, EmptyInputPrintsPlaceholders) {
  ProgramResult run = RunWithInput("");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.out, "(нет занятий)"));
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"\":"));
}

TEST(Application, MissingFilterLineMeansEmptyFilter) {
  ProgramResult run = RunWithInput("2024.03.11 10:00 \"А\"\n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"\":\n(нет занятий)"));
}

TEST(Application, UnknownTeacherGivesNoLessons) {
  ProgramResult run = RunWithInput("2024.03.11 10:00 \"А\"\n", "Б\n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"Б\":\n(нет занятий)"));
}

TEST(Application, FilterNameIsTrimmed) {
  ProgramResult run = RunWithInput("2024.03.11 10:00 \"А\"\n", "   А  \n");
  EXPECT_TRUE(Contains(run.out, "Занятия преподавателя \"А\":\n2024.03.11"));
}

TEST(Application, SortingUsesDateBeforeTime) {
  ProgramResult run = RunWithInput(
      "2024.03.12 08:00 \"А\"\n2024.03.11 18:00 \"Б\"\n", "А\n");
  size_t first = run.out.find("по возрастанию");
  ASSERT_TRUE(first != std::string::npos);
  size_t b = run.out.find("18:00  Б", first);
  size_t a = run.out.find("08:00  А", first);
  EXPECT_TRUE(b < a);
}

TEST(Application, OnlyErrorsStillCompletes) {
  ProgramResult run = RunWithInput("x\ny\n", "А\n");
  EXPECT_EQ(run.code, 0);
  EXPECT_TRUE(Contains(run.err, "Строка 1 пропущена"));
  EXPECT_TRUE(Contains(run.out, "(нет занятий)"));
}
