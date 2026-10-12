#include "lesson_sorting.h"

#include <algorithm>
#include <tuple>

namespace {

// Ключ сравнения: дата, часы, минуты.
std::tuple<Date, int, int> SortKey(const Lesson& lesson) {
  return std::make_tuple(lesson.date, lesson.time.hours, lesson.time.minutes);
}

}  // namespace

bool IsEarlier(const Lesson& a, const Lesson& b) {
  return SortKey(a) < SortKey(b);
}

void SortByTime(std::vector<Lesson>& lessons) {
  std::stable_sort(lessons.begin(), lessons.end(), IsEarlier);
}
