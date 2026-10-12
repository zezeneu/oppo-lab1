// Фильтрация занятий по преподавателю.
#ifndef LESSONS_LESSON_FILTERING_H_
#define LESSONS_LESSON_FILTERING_H_

#include <string>
#include <vector>

#include "lesson.h"

// Возвращает занятия, у которых имя преподавателя точно совпадает с
// teacher_name (с учётом регистра). Порядок занятий сохраняется.
std::vector<Lesson> FilterByTeacher(const std::vector<Lesson>& lessons,
                                    const std::string& teacher_name);

#endif  // LESSONS_LESSON_FILTERING_H_
