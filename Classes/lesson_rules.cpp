#include "lesson_rules.h"

#include "parsing_utils.h"

bool ByStartTime(const Lesson& a, const Lesson& b) { return a.StartsBefore(b); }

LessonList::Predicate ByTeacher(std::string teacher_name) {
  std::string wanted = Trim(teacher_name);
  return [wanted](const Lesson& lesson) {
    return lesson.teacher_name() == wanted;
  };
}
