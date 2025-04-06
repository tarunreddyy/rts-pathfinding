# RTS Pathfinding (Stage 3)

## Overview
This is a small C++ project showcasing:
1. **`Manual JSON Parsing`** – We scan the contents of `layers[0].data` without using external libraries.  
2. **`A* Pathfinding`** – We find the shortest path on a grid, treating cells with a value of `3` as blocked terrain.  
3. **`JSON Output Generation`** – We produce a new JSON file (`output_map.json`) that can be visualized in [RiskyLab Tilemap](https://riskylab.com/tilemap/).

By this stage (Stage 3), we’ve added the A* search routine. The program now:
- Loads `sample_map.json` from the `data` folder.
- Searches for a path between two special cells (e.g., one marked with `0.5` as start, and one with `8.1` as goal).
- Marks the path on the grid as `0.5`.
- Writes the updated map data into `output_map.json` for easy viewing in [RiskyLab Tilemap](https://riskylab.com/tilemap/).

## Requirements
- A **C++17**-compatible compiler (e.g., `g++ 14.2.0` or newer).
- A terminal or command prompt to run your compile commands.

## How to Build
1. Verify you have a compatible compiler (e.g., g++ 14.2.0).
2. From the project root directory, compile the code using:
   ```bash
   g++ -std=c++17 -o rts-pathfinding src/main.cpp src/Map.cpp src/JsonParser.cpp src/Pathfinding.cpp -I./src
   ```
   Adjust the source files as necessary if your project structure differs.

3. **Windows users** can run the provided `compile.bat` to compile in one step.

## How to Run
After a successful build, run:
```bash
rts-pathfinding.exe
```


The executable:
1. Loads `data/sample_map.json`.
2. Prints the map dimensions to confirm successful loading.
3. Runs A* to find a path, marking that path in the map data with the special value `0.5`.
4. Writes the result to `data/output_map.json`.

**Pro Tip:** You can open `output_map.json` in [RiskyLab Tilemap](https://riskylab.com/tilemap/) to visually confirm your path.

## Project Structure
```
rts-pathfinding/
├── src/
│   ├── main.cpp
│   ├── Map.h
│   ├── Map.cpp
│   ├── JsonParser.h
│   ├── JsonParser.cpp
│   ├── Pathfinding.h
│   ├── Pathfinding.cpp
│   ├── Utils.h  
│   └── ...
├── data/
│   ├── sample_map.json
│   └── output_map.json
├── compile.bat
├── README.md
└── ...
```
- `JsonParser.cpp` / `.h`: Manual JSON parsing to extract tilemap data.  
- `Map.cpp` / `.h`: Holds the grid, allows getting/setting cells.  
- `Pathfinding.cpp` / `.h`: Implements the A* search from start to goal.  
- `Utils.h`: Helper functions to mark the path and produce a JSON output.  
- `data/sample_map.json`: Example input map.  
- `data/output_map.json`: Generated map file with the path marked.

## Conclusion
By completing **Stage 3**:
- We’ve integrated custom JSON parsing (`JsonParser`).
- Implemented a **Map** class to store grid data.
- Added **A\* pathfinding** to find and mark a path in your grid.
- Output the updated map in JSON form for visualization.