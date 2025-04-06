/******************************************************************************
 * File:    Pathfinding.cpp
 *
 * Overview:
 *   This file contains the implementation of the Pathfinding class, which uses
 *   the A* algorithm to find a path on a Map from a given start cell to a goal cell.
 * 
 *   Highlights:
 *   - The priority queue (openSet) is ordered by fCost (gCost + hCost).
 *   - gCosts are stored in a vector, indexed by row * width + col.
 *   - cameFrom stores the predecessor of each cell to reconstruct the path.
 *   - We skip out-of-bounds and "blocked" cells to avoid invalid paths.
 * 
 * Author:  Tarun Trilokesh
 * Date:    2025-06-04
 ******************************************************************************/

#include "Pathfinding.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

// Internal structure to represent each node in our search space
struct Node {
    int row, col;
    double gCost;  // Cost from the start node to this node
    double hCost;  // Heuristic estimate from this node to the goal

    // fCost is simply gCost + hCost
    double fCost() const {
        return gCost + hCost;
    }
};

// Comparator to ensure the priority_queue is a min-heap based on fCost
struct NodeComparator {
    bool operator()(const Node& a, const Node& b) const {
        return a.fCost() > b.fCost();
    }
};

/**
 * @brief Heuristic function used in the A* algorithm (Manhattan distance).
 * 
 * @param r1 Row index of the first cell.
 * @param c1 Column index of the first cell.
 * @param r2 Row index of the second cell.
 * @param c2 Column index of the second cell.
 * @return   Estimated distance between the two cells.
 */
double Pathfinding::heuristic(int r1, int c1, int r2, int c2) {
    // Manhattan distance
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

/**
 * @brief Main A* routine to find a path from (startRow, startCol) to (goalRow, goalCol) on the map.
 * 
 * @param map       Reference to the Map object containing the grid data.
 * @param startRow  Row index of the start cell.
 * @param startCol  Column index of the start cell.
 * @param goalRow   Row index of the goal cell.
 * @param goalCol   Column index of the goal cell.
 * 
 * @return A vector of (row, column) pairs that represents the path from start to goal.
 *         Returns an empty vector if no path is found.
 */
std::vector<std::pair<int,int>> Pathfinding::aStar(const Map& map,
                                                    int startRow, int startCol,
                                                    int goalRow, int goalCol)
{
    // Dimensions of the map
    int width  = map.getWidth();
    int height = map.getHeight();

    // Helper lambda to convert (row, col) to a unique index for our arrays
    auto index = [&](int r, int c) {
        return r * width + c;
    };

    // Priority queue storing the nodes to explore, ordered by fCost (lowest first)
    std::priority_queue<Node, std::vector<Node>, NodeComparator> openSet;

    // Initialize gCosts (cost from start to each cell) to "infinity"
    std::vector<double> gCosts(width * height, std::numeric_limits<double>::infinity());

    // Map from current node to its predecessor (for path reconstruction)
    std::unordered_map<int, int> cameFrom;

    // Initialize the start node and push it to our open set
    Node start {
        startRow,
        startCol,
        0.0, // gCost for start is 0
        heuristic(startRow, startCol, goalRow, goalCol) // Estimate to goal
    };
    openSet.push(start);
    gCosts[index(startRow, startCol)] = 0.0;

    // Directions for exploring orthogonal neighbors (up, down, left, right)
    int directions[4][2] = {
        { 0,  1},  // Right
        { 1,  0},  // Down
        { 0, -1},  // Left
        {-1,  0}   // Up
    };

    // Flag to indicate if we found a path
    bool foundPath = false;

    // Core A* loop
    while(!openSet.empty()) {
        // Get the node with the smallest fCost
        Node current = openSet.top();
        openSet.pop();

        // Check if we've reached our goal
        if (current.row == goalRow && current.col == goalCol) {
            foundPath = true;
            break;
        }

        // Explore all four adjacent cells
        for (const auto& dir : directions) {
            int newRow = current.row + dir[0];
            int newCol = current.col + dir[1];

            // Skip cells that are out of bounds
            if (newRow < 0 || newRow >= height || newCol < 0 || newCol >= width) {
                continue;
            }

            // Skip blocked cells
            if (map.getCell(newRow, newCol) == 3.0) {
                continue;
            }

            // Cost to move from current cell to this neighbor (assume uniform cost = 1)
            double newGCost = gCosts[index(current.row, current.col)] + 1;

            // If we found a cheaper path to this neighbor, update and push to openSet
            if (newGCost < gCosts[index(newRow, newCol)]) {
                gCosts[index(newRow, newCol)] = newGCost;
                double hCost = heuristic(newRow, newCol, goalRow, goalCol);

                // Create neighbor node and push to openSet
                Node neighbor { newRow, newCol, newGCost, hCost };
                openSet.push(neighbor);

                // Record our path: "neighbor came from current"
                cameFrom[index(newRow, newCol)] = index(current.row, current.col);
            }
        }
    }

    // If we found the goal, reconstruct the path by tracing back from the goal
    std::vector<std::pair<int, int>> path;
    if (foundPath) {
        // Start from the goal
        int currentIndex = index(goalRow, goalCol);

        // Trace our steps backward until we reach the start
        while (cameFrom.find(currentIndex) != cameFrom.end()) {
            int r = currentIndex / width;
            int c = currentIndex % width;
            path.push_back({r, c});
            currentIndex = cameFrom[currentIndex];
        }

        // Finally add the start cell
        path.push_back({startRow, startCol});

        // Reverse to have the path in correct order: start -> goal
        std::reverse(path.begin(), path.end());
    }
    // Return the path (empty if none was found)
    return path;
}