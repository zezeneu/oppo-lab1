#include "gtest_lite.h"

#include <string>

#include "lesson_list.h"

namespace {

Lesson MakeLesson(int hours, const char* teacher) {
  return Lesson(Date(2024, 3, 11), TimeOfDay(hours, 0), teacher);
}

bool ByHoursDescending(const Lesson& a, const Lesson& b) {
  return b.time() < a.time();
}

bool IsIvanov(const Lesson& lesson) { return lesson.teacher_name() == "Иванов"; }

}  // namespace

TEST(LessonList, NewListIsEmpty) {
  LessonList list;
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0u);
}

TEST(LessonList, AddIncreasesSizeAndKeepsOrder) {
  LessonList list;
  list.Add(MakeLesson(10, "A"));
  list.Add(MakeLesson(9, "B"));
  EXPECT_FALSE(list.empty());
  ASSERT_EQ(list.size(), 2u);
  EXPECT_EQ(list.items()[0].teacher_name(), "A");
  EXPECT_EQ(list.items()[1].teacher_name(), "B");
}

TEST(LessonList, SortByEmptyListDoesNothing) {
  LessonList list;
  list.SortBy(ByHoursDescending);
  EXPECT_TRUE(list.empty());
}

TEST(LessonList, SortByUsesTheGivenComparator) {
  LessonList list;
  list.Add(MakeLesson(9, "A"));
  list.Add(MakeLesson(12, "B"));
  list.Add(MakeLesson(10, "C"));
  list.SortBy(ByHoursDescending);
  ASSERT_EQ(list.size(), 3u);
  EXPECT_EQ(list.items()[0].teacher_name(), "B");
  EXPECT_EQ(list.items()[1].teacher_name(), "C");
  EXPECT_EQ(list.items()[2].teacher_name(), "A");
}

TEST(LessonList, SortByIsStable) {
  LessonList list;
  list.Add(MakeLesson(9, "first"));
  list.Add(MakeLesson(9, "second"));
  list.Add(MakeLesson(9, "third"));
  list.SortBy(ByHoursDescending);
  EXPECT_EQ(list.items()[0].teacher_name(), "first");
  EXPECT_EQ(list.items()[1].teacher_name(), "second");
  EXPECT_EQ(list.items()[2].teacher_name(), "third");
}

TEST(LessonList, FilterKeepsMatchingLessonsInOrder) {
  LessonList list;
  list.Add(MakeLesson(9, "Иванов"));
  list.Add(MakeLesson(10, "Петров"));
  list.Add(MakeLesson(11, "Иванов"));
  LessonList result = list.Filter(IsIvanov);
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result.items()[0].time().hours(), 9);
  EXPECT_EQ(result.items()[1].time().hours(), 11);
}

TEST(LessonList, FilterDoesNotChangeOriginal) {
  LessonList list;
  list.Add(MakeLesson(9, "Иванов"));
  list.Add(MakeLesson(10, "Петров"));
  list.Filter(IsIvanov);
  EXPECT_EQ(list.size(), 2u);
}

TEST(LessonList, FilterWithNoMatchesGivesEmptyList) {
  LessonList list;
  list.Add(MakeLesson(10, "Петров"));
  EXPECT_TRUE(list.Filter(IsIvanov).empty());
}

TEST(LessonList, FilterOfEmptyListIsEmpty) {
  LessonList list;
  EXPECT_TRUE(list.Filter(IsIvanov).empty());
}

TEST(LessonList, FilterAcceptsLambda) {
  LessonList list;
  list.Add(MakeLesson(9, "A"));
  list.Add(MakeLesson(15, "B"));
  LessonList afternoon =
      list.Filter([](const Lesson& l) { return l.time().hours() >= 12; });
  ASSERT_EQ(afternoon.size(), 1u);
  EXPECT_EQ(afternoon.items()[0].teacher_name(), "B");
}

TEST(LessonList, SortByIsStableForLargeLists) {
  LessonList list;
  for (int i = 0; i < 100; ++i) {
    list.Add(MakeLesson(i % 3, std::to_string(i).c_str()));
  }
  list.SortBy(ByHoursDescending);
  ASSERT_EQ(list.size(), 100u);
  for (size_t i = 1; i < list.size(); ++i) {
    const Lesson& prev = list.items()[i - 1];
    const Lesson& cur = list.items()[i];
    if (prev.time().hours() == cur.time().hours()) {
      EXPECT_LT(std::stoi(prev.teacher_name()), std::stoi(cur.teacher_name()));
    }
  }
}
