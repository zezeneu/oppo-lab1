#include <gtest/gtest.h>

#include "errors.h"
#include "lesson.h"

namespace {

Lesson MakeLesson(int day, int hours, int minutes, const char* teacher) {
  return Lesson(Date(2024, 3, day), TimeOfDay(hours, minutes), teacher);
}

}  // namespace

TEST(Lesson, ConstructorStoresFields) {
  Lesson lesson(Date(2024, 3, 11), TimeOfDay(9, 0), "Иванов");
  EXPECT_EQ(lesson.date(), Date(2024, 3, 11));
  EXPECT_EQ(lesson.time(), TimeOfDay(9, 0));
  EXPECT_EQ(lesson.teacher_name(), "Иванов");
}

TEST(Lesson, ConstructorTrimsName) {
  Lesson lesson(Date(2024, 3, 11), TimeOfDay(9, 0), "  Иванов И.И. \t");
  EXPECT_EQ(lesson.teacher_name(), "Иванов И.И.");
}

TEST(Lesson, ConstructorRejectsEmptyName) {
  EXPECT_THROW(Lesson(Date(2024, 3, 11), TimeOfDay(9, 0), ""), RangeError);
}

TEST(Lesson, ConstructorRejectsBlankName) {
  EXPECT_THROW(Lesson(Date(2024, 3, 11), TimeOfDay(9, 0), "   "), RangeError);
}

TEST(Lesson, ParseTypicalLine) {
  Lesson lesson = Lesson::Parse("2023.09.15 14:30 \"Иванов Иван Иванович\"");
  EXPECT_EQ(lesson.date(), Date(2023, 9, 15));
  EXPECT_EQ(lesson.time(), TimeOfDay(14, 30));
  EXPECT_EQ(lesson.teacher_name(), "Иванов Иван Иванович");
}

TEST(Lesson, ParseManySpacesBetweenFields) {
  Lesson lesson = Lesson::Parse("2024.01.01   09:05    \"Петрова А.С.\"");
  EXPECT_EQ(lesson.ToString(), "2024.01.01  09:05  Петрова А.С.");
}

TEST(Lesson, ParseSpacesAtBothEnds) {
  Lesson lesson = Lesson::Parse("   2023.12.31 23:59 \"Сидоров\"   ");
  EXPECT_EQ(lesson.teacher_name(), "Сидоров");
}

TEST(Lesson, ParseTabsAndWindowsLineEnding) {
  Lesson lesson = Lesson::Parse("2023.12.31\t23:59\t\"Сидоров\"\r");
  EXPECT_EQ(lesson.time(), TimeOfDay(23, 59));
}

TEST(Lesson, ParseNumbersWithoutLeadingZerosAreAccepted) {
  Lesson lesson = Lesson::Parse("2023.9.5 7:5 \"Кузнецова\"");
  EXPECT_EQ(lesson.date(), Date(2023, 9, 5));
  EXPECT_EQ(lesson.time(), TimeOfDay(7, 5));
}

TEST(Lesson, ParseSignedNumbersIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.04.30 -10:30 \"Иванов\""), SyntaxError);
  EXPECT_THROW(Lesson::Parse("2023.04.30 03:-30 \"Иванов\""), SyntaxError);
  EXPECT_THROW(Lesson::Parse("2023.04.30 +10:30 \"Иванов\""), SyntaxError);
}

TEST(Lesson, ParseNameBeforeDateIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("\"Иванов\" 2023.01.21 13:20"), SyntaxError);
}

TEST(Lesson, ParseNameWithPunctuationAndDigits) {
  Lesson lesson = Lesson::Parse("2025.02.28 18:45 \"Ли-Смит Д. 2-й\"");
  EXPECT_EQ(lesson.teacher_name(), "Ли-Смит Д. 2-й");
}

TEST(Lesson, ParseEmptyLineIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse(""), SyntaxError);
}

TEST(Lesson, ParseOnlyDateIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15"), SyntaxError);
}

TEST(Lesson, ParseDateAndTimeWithoutNameIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 14:30"), SyntaxError);
}

TEST(Lesson, ParseNameWithoutQuotesIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 14:30 Иванов"), SyntaxError);
}

TEST(Lesson, ParseMissingClosingQuoteIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 14:30 \"Иванов"), SyntaxError);
}

TEST(Lesson, ParseTextAfterNameIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 14:30 \"Иванов\" x"), SyntaxError);
}

TEST(Lesson, ParseSecondNameAfterFirstIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 14:30 \"А\" \"Б\""), SyntaxError);
}

TEST(Lesson, ParseWrongDateFormatIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("15.09.2023 14:30 \"Иванов\""), SyntaxError);
  EXPECT_THROW(Lesson::Parse("2023-09-15 14:30 \"Иванов\""), SyntaxError);
}

TEST(Lesson, ParseWrongTimeFormatIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 1430 \"Иванов\""), SyntaxError);
  EXPECT_THROW(Lesson::Parse("2023.09.15 14.30 \"Иванов\""), SyntaxError);
}

TEST(Lesson, ParseQuotedTextInPlaceOfTimeIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 \"Иванов\""), SyntaxError);
}

TEST(Lesson, ParseImpossibleDateIsRangeError) {
  EXPECT_THROW(Lesson::Parse("2023.02.30 10:00 \"Иванов\""), RangeError);
  EXPECT_THROW(Lesson::Parse("2023.13.01 10:00 \"Иванов\""), RangeError);
}

TEST(Lesson, ParseNonexistentDatesAreRangeError) {
  EXPECT_THROW(Lesson::Parse("2024.02.30 10:00 \"Иванов\""), RangeError);
  EXPECT_THROW(Lesson::Parse("2023.02.29 10:00 \"Иванов\""), RangeError);
  EXPECT_THROW(Lesson::Parse("2023.04.31 10:00 \"Иванов\""), RangeError);
  EXPECT_THROW(Lesson::Parse("0000.01.01 10:00 \"Иванов\""), RangeError);
}

TEST(Lesson, ParseLeapDayIsAccepted) {
  EXPECT_EQ(Lesson::Parse("2024.02.29 10:00 \"Иванов\"").date(),
            Date(2024, 2, 29));
}

TEST(Lesson, ParseNameOfAllowedLength) {
  const std::string name(900, 'x');
  Lesson lesson = Lesson::Parse("2024.03.11 10:00 \"" + name + "\"");
  EXPECT_EQ(lesson.teacher_name().size(), name.size());
}

TEST(Lesson, ParseTooLongLineIsSyntaxError) {
  const std::string name(200000, 'x');
  EXPECT_THROW(Lesson::Parse("2024.03.11 10:00 \"" + name + "\""), SyntaxError);
}

TEST(Lesson, ParseImpossibleTimeIsRangeError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 24:00 \"Иванов\""), RangeError);
  EXPECT_THROW(Lesson::Parse("2023.09.15 12:60 \"Иванов\""), RangeError);
}

TEST(Lesson, ParseEmptyNameIsSyntaxError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 10:00 \"\""), SyntaxError);
}

TEST(Lesson, ParseBlankNameIsRangeError) {
  EXPECT_THROW(Lesson::Parse("2023.09.15 10:00 \"   \""), RangeError);
}

TEST(Lesson, ParseErrorsCanBeCaughtAsLessonError) {
  EXPECT_THROW(Lesson::Parse("мусор"), LessonError);
  EXPECT_THROW(Lesson::Parse("2023.09.15 10:00 \"\""), LessonError);
}

TEST(Lesson, StartsBeforeComparesTimeOnSameDay) {
  EXPECT_TRUE(MakeLesson(11, 9, 0, "A").StartsBefore(MakeLesson(11, 10, 0, "B")));
  EXPECT_FALSE(MakeLesson(11, 10, 0, "A").StartsBefore(MakeLesson(11, 9, 0, "B")));
}

TEST(Lesson, StartsBeforeComparesDateFirst) {
  EXPECT_TRUE(MakeLesson(11, 18, 0, "A").StartsBefore(MakeLesson(12, 8, 0, "B")));
  EXPECT_FALSE(MakeLesson(12, 8, 0, "A").StartsBefore(MakeLesson(11, 18, 0, "B")));
}

TEST(Lesson, StartsBeforeIsFalseForSameMoment) {
  EXPECT_FALSE(MakeLesson(11, 9, 0, "A").StartsBefore(MakeLesson(11, 9, 0, "B")));
}

TEST(Lesson, ToStringFormat) {
  EXPECT_EQ(MakeLesson(5, 8, 5, "Иванов И.И.").ToString(),
            "2024.03.05  08:05  Иванов И.И.");
}
