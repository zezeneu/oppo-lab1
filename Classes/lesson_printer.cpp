#include "lesson_printer.h"

void LessonPrinter::PrintSection(const std::string& title,
                                 const LessonList& lessons) const {
  out_ << '\n' << title << '\n';
  if (lessons.empty()) {
    out_ << "(нет занятий)\n";
  }
  for (const Lesson& lesson : lessons.items()) {
    out_ << lesson.ToString() << '\n';
  }
}

void LessonPrinter::PrintErrors(const std::vector<LineError>& errors) const {
  for (const LineError& error : errors) {
    out_ << "Строка " << error.line_number << " пропущена: " << error.message
         << '\n';
  }
}
