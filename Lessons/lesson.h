// Объект «Учебное занятие» и его разбор из текстовой строки.
#ifndef LESSONS_LESSON_H_
#define LESSONS_LESSON_H_

#include <string>

#include "date_time.h"

struct Lesson {
  Date date;
  Time time;
  std::string teacher_name;
};

// Преобразует строку вида: гггг.мм.дд чч:мм "Имя преподавателя" в Lesson.
// При ошибке формата бросает std::runtime_error.
Lesson ParseLesson(const std::string& line);

#endif  // LESSONS_LESSON_H_
