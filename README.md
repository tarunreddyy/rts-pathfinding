# rts-pathfinding

## Overview
A C++ project that:
- Parses a grid-based map from JSON with a custom parser.
- Performs A* pathfinding.
- Outputs a new JSON containing the path, compatible with RiskyLab Tilemap.

## Requirements
- C++17 or newer (confirmed with g++.exe 14.2.0).
- A terminal to run `g++`.

## How to Build
Example command to compile (check g++ version):
```bash
g++ -std=c++17 -o rts-pathfinding src/main.cpp -I./src
```
or in the command line (Windows) run below:
```bash
compile.bat
```

## How to Run
After building in the command line (Windows) run below:
```bash
rts-pathfinding.exe
```

## Project Structure
- src/: Source code
- data/: Contains sample_map.json and the generated output_map.json.