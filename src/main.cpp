/******************************************************************************
 * File: main.cpp
 *
 * Description:
 *   This file serves as the entry point for the RTS Pathfinding Project.
 *   In this stage, we focus on testing custom JSON parsing by:
 *     1) Loading a sample map from "sample_map.json".
 *     2) Printing the map's dimensions and contents to confirm success.
 *
 *   Future stages may expand this code to include A* pathfinding logic
 *   and generation of updated JSON for visualization.
 *
 * Author: Tarun Trilokesh
 * Date:   2025-03-04
 *
 ******************************************************************************/

#include <iostream>
#include "Map.h"

int main() {
    std::cout << "RTS Pathfinding - Stage 2 (Custom JSON parsing testing)\n";

    // Create a Map object and attempt to load from sample_map.json
    Map map;
    if (!map.loadFromJson("./data/sample_map.json")) {
        std::cerr << "Failed to load map from file.\n";
        return 1;
    }

    // Print dimensions
    std::cout << "Map loaded successfully! "
              << "Width = " << map.getWidth()
              << ", Height = " << map.getHeight() << "\n";

    // Display the entire grid
    std::cout << "Map data:\n";
    for (int r = 0; r < map.getHeight(); ++r) {
        for (int c = 0; c < map.getWidth(); ++c) {
            std::cout << map.getCell(r, c) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Map data loaded successfully!\n";

    // Print a specific cell value (example: row 2, column 3)
    int row = 0, col = 4;
    try {
        int cellValue = map.getCell(row, col);
        std::cout << "Value at (" << row << ", " << col << ") = " << cellValue << "\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
} // end of main
// end of main.cpp 