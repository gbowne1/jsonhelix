@echo off
setlocal

:: Set environment variables
set CC=cl
set CXX=cl

:: Build the project
cmake -S . -B build
cmake --build build

:: Run the application
build\JsonHelix.exe

endlocal
