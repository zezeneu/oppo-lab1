/// @file
/// @brief Готовые критерии сортировки и фильтрации занятий. Новый критерий
/// добавляется новой функцией, класс LessonList при этом не меняется.
#ifndef LESSONS_LESSON_RULES_H_
#define LESSONS_LESSON_RULES_H_

#include <string>

#include "lesson_list.h"

/// Порядок «по времени начала»: сначала дата, затем время.
bool ByStartTime(const Lesson& a, const Lesson& b);

/// Отбор занятий, у которых имя преподавателя точно равно teacher_name
/// (с учётом регистра; пробелы по краям имени не учитываются).
LessonList::Predicate ByTeacher(const std::string& teacher_name);

#endif  // LESSONS_LESSON_RULES_H_
