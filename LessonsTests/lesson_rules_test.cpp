#include "gtest_lite.h"

#include "lesson_rules.h"

namespace {

Lesson MakeLesson(int day, int hours, const char* teacher) {
  return Lesson(Date(2024, 3, day), TimeOfDay(hours, 0), teacher);
}

LessonList MakeSampleList() {
  LessonList list;
  list.Add(MakeLesson(12, 8, "Иванов И.И."));
  list.Add(MakeLesson(11, 14, "Петров П.П."));
  list.Add(MakeLesson(11, 9, "Иванов И.И."));
  list.Add(MakeLesson(11, 9, "Сидоров С.С."));
  return list;
}

}  // namespace

TEST(ByStartTime, EarlierTimeComesFirst) {
  EXPECT_TRUE(ByStartTime(MakeLesson(11, 9, "A"), MakeLesson(11, 10, "B")));
  EXPECT_FALSE(ByStartTime(MakeLesson(11, 10, "A"), MakeLesson(11, 9, "B")));
}

TEST(ByStartTime, DateIsMoreImportantThanTime) {
  EXPECT_TRUE(ByStartTime(MakeLesson(11, 20, "A"), MakeLesson(12, 8, "B")));
}

TEST(ByStartTime, SortsListChronologically) {
  LessonList list = MakeSampleList();
  list.SortBy(ByStartTime);
  ASSERT_EQ(list.size(), 4u);
  EXPECT_EQ(list.items()[0].teacher_name(), "Иванов И.И.");
  EXPECT_EQ(list.items()[0].time().hours(), 9);
  EXPECT_EQ(list.items()[1].teacher_name(), "Сидоров С.С.");
  EXPECT_EQ(list.items()[2].teacher_name(), "Петров П.П.");
  EXPECT_EQ(list.items()[3].date().day(), 12);
}

TEST(ByStartTime, EqualTimesKeepInputOrder) {
  LessonList list = MakeSampleList();
  list.SortBy(ByStartTime);
  EXPECT_EQ(list.items()[0].teacher_name(), "Иванов И.И.");
  EXPECT_EQ(list.items()[1].teacher_name(), "Сидоров С.С.");
}

TEST(ByTeacher, KeepsOnlyExactMatches) {
  LessonList result = MakeSampleList().Filter(ByTeacher("Иванов И.И."));
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result.items()[0].date().day(), 12);
  EXPECT_EQ(result.items()[1].date().day(), 11);
}

TEST(ByTeacher, IsCaseSensitive) {
  EXPECT_TRUE(MakeSampleList().Filter(ByTeacher("иванов и.и.")).empty());
}

TEST(ByTeacher, DoesNotMatchPartOfName) {
  EXPECT_TRUE(MakeSampleList().Filter(ByTeacher("Иванов")).empty());
}

TEST(ByTeacher, IgnoresSpacesAroundWantedName) {
  EXPECT_EQ(MakeSampleList().Filter(ByTeacher("  Петров П.П. ")).size(), 1u);
}

TEST(ByTeacher, EmptyNameMatchesNothing) {
  EXPECT_TRUE(MakeSampleList().Filter(ByTeacher("")).empty());
}

TEST(ByTeacher, UnknownTeacherMatchesNothing) {
  EXPECT_TRUE(MakeSampleList().Filter(ByTeacher("Кузнецов К.К.")).empty());
}
