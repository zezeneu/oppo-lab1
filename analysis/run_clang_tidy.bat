@echo off
rem Статический анализ clang-tidy (настройки берутся из файла .clang-tidy).
rem Нужен LLVM:  winget install LLVM.LLVM
chcp 65001 >nul
cd /d "%~dp0.."
clang-tidy Classes\*.cpp Lessons\main.cpp -- -std=c++20 -IClasses > analysis\clang_tidy_report.txt 2>&1
type analysis\clang_tidy_report.txt
