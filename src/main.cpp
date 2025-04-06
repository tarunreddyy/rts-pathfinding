/******************************************************************************
 * File:    main.cpp
 *
 * Overview:
 *   This file is the entry point for the RTS Pathfinding Project. In this stage,
 *   it tests the custom JSON parsing by:
 *     1) Loading a sample map from "data/sample_map.json".
 *     2) Printing the map’s dimensions to confirm successful loading.
 *     3) Running an A* search from a cell with value 0.5 to a cell with value 8.1.
 *     4) Marking the found path on the map and writing the result to "data/output_map.json".
 *
 * Author:  Tarun Trilokesh
 * Date:    2025-06-04
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "Map.h"
#include "Pathfinding.h"
#include "Utils.h"

int main() {
    std::cout << "RTS Pathfinding - Stage 3 (Path Planning A* algorithm testing with JSON output)\n";

    // Create a Map object and attempt to load JSON data from sample_map.json
    Map map;
    if (!map.loadFromJson("./data/sample_map.json")) {
        std::cerr << "Failed to load map from file.\n";
        return 1;
    }

    // Print basic map information to confirm it loaded correctly
    std::cout << "Map loaded successfully! "
              << "Width = " << map.getWidth()
              << ", Height = " << map.getHeight() << "\n";

    std::cout << "Map data loaded successfully!\n";

    /////////////////////// Search for a specific value (example: 0.5)
    // double searchValue = 0.5;
    // bool found = false;
    // for (int r = 0; r < map.getHeight(); ++r) {
    //     for (int c = 0; c < map.getWidth(); ++c) {
    //         if (map.getCell(r, c) == searchValue) {
    //             std::cout << "Found value " << searchValue << " at (" << r << ", " << c << ")\n";
    //             found = true;
    //             break;
    //         }
    //     }
    //     if (found) break;
    // }

    /////////////////////// Print a specific cell value (example: row 2, column 3)
    // int row = 0, col = 4;
    // try {
    //     int cellValue = map.getCell(row, col);
    //     std::cout << "Value at (" << row << ", " << col << ") = " << cellValue << "\n";
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Error: " << e.what() << "\n";
    // }

    // Variables to store the coordinates of the start and goal cells
    int startRow = -1, startCol = -1;
    int goalRow  = -1, goalCol  = -1;

    // Locate the cells with specific values that denote a start (0.5) and a goal (8.1)
    for (int r = 0; r < map.getHeight(); ++r) {
        for (int c = 0; c < map.getWidth(); ++c) {
            double cellValue = map.getCell(r, c);
            if (cellValue == 0.5) {
                startRow = r;
                startCol = c;
            } else if (cellValue == 8.1) {
                goalRow = r;
                goalCol = c;
            }
        }
    }

    // Use A* pathfinding to get a path from start to goal
    auto path = Pathfinding::aStar(map, startRow, startCol, goalRow, goalCol);

    // If the path is empty, no route was found
    if (path.empty()) {
        std::cout << "No path found.\n";
    } else {
        std::cout << "Path found! Steps:\n";
        for (auto &step : path) {
            std::cout << "(" << step.first << "," << step.second << ") -> ";
        }
        std::cout << "GOAL\n";

        // Mark the path on the map (changes map internally)
        markPathOnMap(map, path);
        std::cout << "Path length: " << path.size() << "\n";
    }

    // Generate a new JSON string reflecting the path-marked map
    std::string outputJson = generateJsonOutput(map, "data/sample_map.json");

    // Write the updated map to an output file
    std::ofstream fout("data/output_map.json");
    if (!fout) {
        std::cerr << "Could not open data/output_map.json for writing.\n";
        return 1;
    }
    fout << outputJson;
    fout.close();

    std::cout << "Wrote updated map with path to data/output_map.json\n";
    std::cout << "Load it in your tilemap viewer to visualize the path.\n";

    return 0;
}
 