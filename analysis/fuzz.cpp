// Мутационный фаззер: портит корректные строки занятий и проверяет, что разбор
// не падает и не нарушает память (запускать со включённым AddressSanitizer).
#include <cstdio>
#include <random>
#include <sstream>
#include <string>
#include "application.h"
#include "errors.h"
#include "lesson.h"
int main(int argc, char** argv) {
  unsigned seed = argc > 1 ? std::stoul(argv[1]) : 1;
  int iters = argc > 2 ? std::stoi(argv[2]) : 200000;
  std::mt19937 rng(seed);
  const std::string base[] = {"2023.09.15 10:30 \"Иванов И. И.\"", "2024.02.29 00:00 \"A\"", "2023.12.31 23:59 \"  Петров  \""};
  const std::string alpha = " \t\r\n\"0123456789.:-+ёЯabc\xff\xfe\x80\x00";
  long ok = 0, bad = 0;
  for (int i = 0; i < iters; ++i) {
    std::string s = base[rng() % 3];
    int m = 1 + rng() % 4;
    for (int k = 0; k < m; ++k) {
      size_t pos = s.empty() ? 0 : rng() % s.size();
      switch (rng() % 4) {
        case 0: if (!s.empty()) s.erase(pos, 1 + rng() % 3); break;
        case 1: s.insert(pos, 1, alpha[rng() % alpha.size()]); break;
        case 2: if (!s.empty()) s[pos] = alpha[rng() % alpha.size()]; break;
        default: s += s.substr(0, rng() % (s.size() + 1)); break;
      }
    }
    try { Lesson l = Lesson::Parse(s); (void)l.ToString(); ++ok; }
    catch (const LessonError&) { ++bad; }
  }
  std::printf("seed=%u parsed=%ld rejected=%ld\n", seed, ok, bad);
  // Полный сценарий на случайном вводе
  for (int i = 0; i < 2000; ++i) {
    std::string in;
    for (int j = 0, n = rng() % 20; j < n; ++j) in += base[rng() % 3] + (rng() % 5 ? "\n" : "\n\n");
    std::istringstream is(in); std::ostringstream os, es;
    Application app(is, os, es); app.Run();
  }
  std::puts("app scenarios ok");
}
