/// @file
/// @brief Коллекция занятий с сортировкой и фильтрацией.
#ifndef LESSONS_LESSON_LIST_H_
#define LESSONS_LESSON_LIST_H_

#include <functional>
#include <vector>

#include "lesson.h"

class LessonList {
 public:
  /// Критерий отбора: true, если занятие нужно оставить.
  using Predicate = std::function<bool(const Lesson&)>;
  /// Критерий порядка: true, если первое занятие должно идти раньше второго.
  using Comparator = std::function<bool(const Lesson&, const Lesson&)>;

  void Add(Lesson lesson) { items_.push_back(std::move(lesson)); }
  size_t size() const { return items_.size(); }
  bool empty() const { return items_.empty(); }
  const std::vector<Lesson>& items() const { return items_; }

  /// Устойчивая сортировка: равные по критерию занятия сохраняют порядок.
  void SortBy(const Comparator& comes_first);

  /// Возвращает новый список из занятий, для которых keep вернул true.
  LessonList Filter(const Predicate& keep) const;

 private:
  std::vector<Lesson> items_{};
};

#endif  // LESSONS_LESSON_LIST_H_
