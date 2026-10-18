@echo off
rem Документация по исходному коду. Нужны Doxygen и Graphviz:
rem   winget install DimitriVanHeesch.Doxygen
rem   winget install Graphviz.Graphviz
chcp 65001 >nul
cd /d "%~dp0..\docs"
doxygen Doxyfile
start "" generated\html\index.html
