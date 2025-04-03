# RTS Pathfinding (Stage 2)

## Overview
This project demonstrates:
- `Custom JSON parsing` (using a naive parser) to load a grid-based map.
- Basic printing of the map to verify correct loading.

Later stages will:
- Implement A* pathfinding.
- Output a new JSON for visualization in [RiskyLab Tilemap](https://riskylab.com/tilemap/).

## Requirements
- `C++17` or newer (example: `g++.exe 14.2.0`).
- A terminal or command prompt to run `g++`.

## How to Build
1. Ensure you have a compatible compiler (e.g., g++ 14.2.0).
2. From the project root, compile using:
   ```bash
   g++ -std=c++17 -o rts-pathfinding src/main.cpp src/Map.cpp src/JsonParser.cpp -I./src
   ```
   `(Adjust files as needed if your source list differs.)`

3. `(Windows users)` can alternatively run:
   ```bash
   compile.bat
   ```

## How to Run
After compiling:
```bash
rts-pathfinding.exe
```
- The program attempts to load `sample_map.json` in the current working directory.
- It then prints the map dimensions and contents to confirm that the parsing works.

## Project Structure
```
rts-pathfinding/
├── src/
│   ├── main.cpp
│   ├── Map.h
│   ├── Map.cpp
│   ├── JsonParser.h
│   ├── JsonParser.cpp
│   └── ...
├── data/
│   ├── sample_map.json
│   └── output_map.json (generated in future stages)
├── compile.bat (optional, for Windows)
├── README.md (this file)
└── ...
```

- `src/main.cpp`: Entry point, demonstrates loading and printing map data.
- `src/Map.*`: Loads a grid from JSON and provides access to its cells.
- `src/JsonParser.*`: Naive string-based parsing of the `layers[0].data` array from the JSON.
- `data/sample_map.json`: The sample map used by Stage 2.