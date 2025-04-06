#pragma once

/******************************************************************************
 * File:    Pathfinding.h
 *
 * Overview:
 *   This header file declares the Pathfinding class, which houses the A* pathfinding
 *   routine. A* attempts to find the shortest path on a provided Map object
 *   from a given start cell to a goal cell, using a heuristic to guide the search.
 *
 * Author:  Tarun Trilokesh
 * Date:    2025-06-04
 ******************************************************************************/

#include "Map.h"
#include <vector>
#include <utility>

/**
 * @class Pathfinding
 *
 * @brief Contains static methods for performing pathfinding on a Map.
 */
class Pathfinding {
public:
    /**
     * @brief Run the A* algorithm on the given map.
     * 
     * @param map       Reference to the Map object.
     * @param startRow  Row index of the start cell.
     * @param startCol  Column index of the start cell.
     * @param goalRow   Row index of the goal cell.
     * @param goalCol   Column index of the goal cell.
     * @return          A vector of (row, column) pairs representing the path
     *                  from start to goal. Empty if no path is found.
     */
    static std::vector<std::pair<int, int>> aStar(const Map& map,
                                                  int startRow,
                                                  int startCol,
                                                  int goalRow,
                                                  int goalCol);

private:
    /**
     * @brief Heuristic function (e.g., Manhattan distance) used by A*.
     * 
     * @param r1 Row index of the first cell.
     * @param c1 Column index of the first cell.
     * @param r2 Row index of the second cell.
     * @param c2 Column index of the second cell.
     * @return   The estimated cost (distance) between the two cells.
     */
    static double heuristic(int r1, int c1, int r2, int c2);
};
