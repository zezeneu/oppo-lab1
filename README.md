# Учебные занятия: дата, время, имя преподавателя

Строка ввода: `гггг.мм.дд чч:мм "Имя преподавателя"`, например
`2024.03.11 14:30 "Иванов И.И."`.

Требуется Visual Studio 2022 (стандарт C++20: нужен `std::chrono::year_month_day`).
Разбор строки — одно регулярное выражение `std::regex`; существование даты
(30 февраля, 31 апреля, 29 февраля не в високосный год) проверяет
`std::chrono::year_month_day::ok()`.

## Как открыть и запустить
Дважды щёлкните `Lessons.sln`, конфигурация **Debug**, платформа **x64**, затем **Ctrl+F5**.

Работа 5: к работе 3 добавлены анализ кода и документация, исправлен дефект
с меткой BOM в начале файла и сделаны небольшие оптимизации.

## Запуск тестов
ПКМ по проекту **LessonsTests** → «Назначить запускаемым проектом» → **Ctrl+F5**.
В конце должно быть `[  PASSED  ] 116 tests`.

## Из командной строки (Developer Command Prompt for VS 2022)
```bat
chcp 65001
cl /EHsc /std:c++20 /utf-8 /IClasses Lessons\main.cpp Classes\*.cpp /Fe:Lessons.exe
cl /EHsc /std:c++20 /utf-8 /IClasses /ILessonsTests LessonsTests\*.cpp Classes\date.cpp Classes\time_of_day.cpp Classes\lesson.cpp Classes\lesson_list.cpp Classes\lesson_rules.cpp Classes\lesson_reader.cpp Classes\lesson_printer.cpp Classes\application.cpp Classes\parsing_utils.cpp /Fe:LessonsTests.exe
LessonsTests.exe
```

## Анализ кода (работа 5)
Папки и файлы, добавленные в работе 5:
- `.clang-tidy` — набор проверок статического анализатора;
- `docs/Doxyfile` — настройки генератора документации;
- `analysis/` — скрипты и фаззер для динамического анализа.

### Статический анализ
- Visual Studio 2022: **Анализ → Запуск анализа кода для решения** (набор правил C++ Core Check).
- clang-tidy: установите LLVM (`winget install LLVM.LLVM`), запустите `analysis\run_clang_tidy.bat`.

### Динамический анализ
- AddressSanitizer: свойства проекта LessonsTests → C/C++ → Общие → **Включить AddressSanitizer = Да**,
  инкрементальную компоновку отключить (`/INCREMENTAL:NO`), конфигурация Debug x64, запуск.
- Фаззер: добавить `analysis\fuzz.cpp` в отдельный проект-консольное приложение (подключить `Classes\*.cpp`) и запускать с ASan.

### Профилирование
`analysis\make_big_input.ps1` создаёт `big.txt`. Запуск: **Отладка → Профилировщик производительности (Alt+F2)** →
«Использование ЦП» и «Использование памяти», перенаправление ввода: аргументы команды `< big.txt`
(в свойствах проекта → Отладка → Аргументы команды: `< $(ProjectDir)..\analysis\big.txt`).

### Документация
`analysis\make_docs.bat` (нужны Doxygen и Graphviz) — результат в `docs\generated\html\index.html`.
