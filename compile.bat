@echo off
g++ -std=c++17 -o rts-pathfinding src/main.cpp src/Map.cpp src/JsonParser.cpp -I./src
if %errorlevel% equ 0 (
    echo Compiled successfully.
) else (
    echo Compilation failed.
)
pause