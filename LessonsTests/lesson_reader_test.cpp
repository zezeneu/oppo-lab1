#include <sstream>

#include "gtest_lite.h"

#include "lesson_reader.h"

TEST(LessonReader, ReadsAllValidLinesUntilBlankLine) {
  std::istringstream in("2024.03.11 09:00 \"A\"\n2024.03.11 10:00 \"B\"\n\nrest\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.lessons.size(), 2u);
  EXPECT_TRUE(result.errors.empty());
}

TEST(LessonReader, StopsAtEndOfInputWithoutBlankLine) {
  std::istringstream in("2024.03.11 09:00 \"A\"\n2024.03.11 10:00 \"B\"");
  EXPECT_EQ(LessonReader().ReadAll(in).lessons.size(), 2u);
}

TEST(LessonReader, EmptyInputGivesEmptyResult) {
  std::istringstream in("");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_TRUE(result.lessons.empty());
  EXPECT_TRUE(result.errors.empty());
}

TEST(LessonReader, BlankFirstLineEndsReadingImmediately) {
  std::istringstream in("\n2024.03.11 09:00 \"A\"\n");
  EXPECT_TRUE(LessonReader().ReadAll(in).lessons.empty());
}

TEST(LessonReader, LineWithOnlySpacesEndsReading) {
  std::istringstream in("2024.03.11 09:00 \"A\"\n   \t \n2024.03.11 10:00 \"B\"\n");
  EXPECT_EQ(LessonReader().ReadAll(in).lessons.size(), 1u);
}

TEST(LessonReader, BadLineIsRecordedAndReadingContinues) {
  std::istringstream in("2024.03.11 09:00 \"A\"\nмусор\n2024.03.11 10:00 \"B\"\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.lessons.size(), 2u);
  ASSERT_EQ(result.errors.size(), 1u);
  EXPECT_EQ(result.errors[0].line_number, 2u);
}

TEST(LessonReader, ErrorMessageComesFromException) {
  std::istringstream in("2024.02.30 09:00 \"A\"\n");
  ReadResult result = LessonReader().ReadAll(in);
  ASSERT_EQ(result.errors.size(), 1u);
  EXPECT_TRUE(result.errors[0].message.find("календаре") != std::string::npos);
}

TEST(LessonReader, AllLinesBadGivesOnlyErrors) {
  std::istringstream in("a\nb\nc\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_TRUE(result.lessons.empty());
  ASSERT_EQ(result.errors.size(), 3u);
  EXPECT_EQ(result.errors[2].line_number, 3u);
}

TEST(LessonReader, SyntaxAndRangeErrorsAreBothHandled) {
  std::istringstream in("2024.03.11 09:00 Иванов\n2024.03.11 09:00 \"\"\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.errors.size(), 2u);
}

TEST(LessonReader, WindowsLineEndingsAreAccepted) {
  std::istringstream in("2024.03.11 09:00 \"A\"\r\n2024.03.11 10:00 \"B\"\r\n\r\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.lessons.size(), 2u);
  EXPECT_TRUE(result.errors.empty());
}

TEST(LessonReader, IgnoresUtf8BomAtStartOfInput) {
  std::istringstream in("\xEF\xBB\xBF" "2024.03.11 09:00 \"A\"\n2024.03.11 10:00 \"B\"\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.lessons.size(), 2u);
  EXPECT_TRUE(result.errors.empty());
}

TEST(LessonReader, BomInsideLaterLineIsStillAnError) {
  std::istringstream in("2024.03.11 09:00 \"A\"\n\xEF\xBB\xBF" "2024.03.11 10:00 \"B\"\n");
  ReadResult result = LessonReader().ReadAll(in);
  EXPECT_EQ(result.lessons.size(), 1u);
  EXPECT_EQ(result.errors.size(), 1u);
}

TEST(LessonReader, BomBeforeBlankLineEndsReading) {
  std::istringstream in("\xEF\xBB\xBF\n2024.03.11 09:00 \"A\"\n");
  EXPECT_TRUE(LessonReader().ReadAll(in).lessons.empty());
}
