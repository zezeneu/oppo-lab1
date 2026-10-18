#include "lesson_list.h"

#include <algorithm>

void LessonList::SortBy(const Comparator& comes_first) {
  std::stable_sort(items_.begin(), items_.end(), comes_first);
}

LessonList LessonList::Filter(const Predicate& keep) const {
  LessonList result;
  for (const Lesson& lesson : items_) {
    if (keep(lesson)) {
      result.Add(lesson);
    }
  }
  return result;
}
