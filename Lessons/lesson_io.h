// Ввод и вывод списка занятий.
#ifndef LESSONS_LESSON_IO_H_
#define LESSONS_LESSON_IO_H_

#include <istream>
#include <string>
#include <vector>

#include "lesson.h"

// Читает строки до пустой строки (или конца ввода) и разбирает каждую в
// Lesson. Строки с ошибкой пропускаются с сообщением в std::cerr.
std::vector<Lesson> ReadAllLessons(std::istream& in);

// Печатает одно занятие: дата, время, преподаватель.
void PrintLesson(const Lesson& lesson);

// Печатает заголовок и все занятия списка (или «нет занятий»).
void PrintSection(const std::string& title, const std::vector<Lesson>& lessons);

#endif  // LESSONS_LESSON_IO_H_
