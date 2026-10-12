#include "lesson_filtering.h"

std::vector<Lesson> FilterByTeacher(const std::vector<Lesson>& lessons,
                                    const std::string& teacher_name) {
  std::vector<Lesson> result;
  for (const Lesson& lesson : lessons) {
    if (lesson.teacher_name == teacher_name) {
      result.push_back(lesson);
    }
  }
  return result;
}
