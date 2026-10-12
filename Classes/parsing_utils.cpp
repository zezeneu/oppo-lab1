#include "parsing_utils.h"

#include "errors.h"

std::string Trim(const std::string& text) {
  const char* const kSpaces = " \t\r";
  const size_t first = text.find_first_not_of(kSpaces);
  if (first == std::string::npos) {
    return "";
  }
  const size_t last = text.find_last_not_of(kSpaces);
  return text.substr(first, last - first + 1);
}

void CheckRange(int value, int min, int max, const std::string& what) {
  if (value < min || value > max) {
    throw RangeError(what + " вне диапазона " + std::to_string(min) + ".." +
                     std::to_string(max) + ": " + std::to_string(value));
  }
}
