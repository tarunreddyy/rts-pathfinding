/******************************************************************************
 * File:    main.cpp
 *
 * Overview:
 *   This file is the entry point for the RTS Pathfinding Project.
 *   1) Load a map from JSON.
 *   2) Detect agents (start values 0.5, 0.6, 0.9) and goals (8.1, 8.4, 8.13).
 *   3) Each agent chooses its nearest goal. Multiple agents can share a goal.
 *   4) Plan A* paths for each agent (if no path is found, that agent remains idle).
 *   5) Mark each agent's path in the map using the agent's start value.
 *   6) Step the agents in a collision-free manner, if desired.
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
#include "MultiUnitCoordinator.h"

int main(int argc, char* argv[]) {
    std::cout << "RTS Pathfinding\n";

    // Parse Command-Line Arguments
    std::string inputFile  = "./data/single_unit_single_goal_test.json";   // default input
    std::string outputFile = "data/output_map.json";   // default output

    if (argc > 1) {
        inputFile = argv[1];
    }
    if (argc > 2) {
        outputFile = argv[2];
    }

    // Create a Map object and attempt to load JSON data from sample_map.json
    Map map;
    if (!map.loadFromJson(inputFile)) {
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

    // Create the multi-unit coordinator
    MultiUnitCoordinator coordinator(map);

    // Detect agent start positions and possible goals
    coordinator.findStartsAndGoals();

    // Assign each agent to its nearest goal (multiple agents can share)
    coordinator.assignGoals();

    // Compute A* paths for each agent
    coordinator.planPaths();

    // Mark each agent's path on the map using its start value
    // Agents with no path found won't mark anything.
    coordinator.markPathsOnMap();

    //Export or print the updated map if you want to see the markings
    std::string updatedJson = generateJsonOutput(map, inputFile);
    std::ofstream out(outputFile);
    out << updatedJson;
    out.close();
    std::cout << "Wrote updated map with paths to data folder.\n";
    

    // Step the agents for a demonstration of collision-free movement
    // const int MAX_STEPS = 50;
    // for (int step = 0; step < MAX_STEPS; ++step) {
    //     std::cout << "Step " << step << ":\n";
    //     coordinator.printAgents();

    //     coordinator.step(); // each agent attempts to move 1 step

    //     if (coordinator.allArrived()) {
    //         std::cout << "All agents that found paths have arrived.\n";
    //         break;
    //     }
    // }

    return 0;
}
 