// Сортировка занятий по времени.
#ifndef LESSONS_LESSON_SORTING_H_
#define LESSONS_LESSON_SORTING_H_

#include <vector>

#include "lesson.h"

// Возвращает true, если занятие a начинается раньше занятия b
// (сравниваются дата, затем время).
bool IsEarlier(const Lesson& a, const Lesson& b);

// Сортирует занятия по возрастанию времени начала. Сортировка устойчива:
// занятия с одинаковым временем сохраняют порядок ввода.
void SortByTime(std::vector<Lesson>& lessons);

#endif  // LESSONS_LESSON_SORTING_H_
