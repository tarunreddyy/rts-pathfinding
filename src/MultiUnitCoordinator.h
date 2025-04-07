/******************************************************************************
 * File: MultiUnitCoordinator.h
 *
 * Description:
 *   Handles up to 3 agents (start values: 0.5, 0.6, 0.9) and up to 3 goals
 *   (8.1, 8.4, 8.13). Each agent is assigned the nearest unclaimed goal.
 *   Then, each agent pathfinds and moves step-by-step, avoiding collisions
 *   by waiting if the next cell is occupied.
 *
 * Author: Tarun Trilokesh
 * Date:   2025-06-04
 ******************************************************************************/

#pragma once

#include "Map.h"
#include "Pathfinding.h"
#include <vector>
#include <utility>

/**
 * @struct Agent
 * @brief Represents an agent in the multi-unit coordination system.
 * 
 * This structure holds the agent's ID, current position, assigned goal,
 * and the path it will follow to reach that goal.
 * 
 * @var id Unique identifier for the agent (0, 1, or 2).
 * @var startVal Unique start value of the agent (0.5, 0.6, or 0.9).
 * @var row Current row position of the agent.
 * @var col Current column position of the agent.
 * @var goalRow Row position of the agent's assigned goal.
 * @var goalCol Column position of the agent's assigned goal.
 * @var path Full path from start to goal, computed using A*.
 * @var pathIndex Current index in the path, indicating the agent's position.
 */
struct Agent {
    int id;
    double startVal;                      // The agent's unique start value
    int row, col;                         // Current position
    int goalRow, goalCol;                 // Assigned goal
    std::vector<std::pair<int,int>> path; // A* path, if found
    int pathIndex = 0;
};



class MultiUnitCoordinator {
public:
    /**
     * Constructor referencing the map. The map is used to:
     *  - Identify agent start cells and goal cells
     *  - Check collisions and run pathfinding
     */
    explicit MultiUnitCoordinator(Map& mapRef);

    /**
     * 1) Finds all starts in the map with values {0.5, 0.6, 0.9}
     * 2) Finds all goals in the map with values {8.1, 8.4, 8.13}
     * Stores them in 'agents' and 'goalCells'.
     */
    void findStartsAndGoals();

    /**
     * Assigns each agent the single nearest goal.
     * 
     * Agents do NOT exclusively claim goals, so multiple agents
     * can share the same target.
     */
    void assignGoals();

    /**
     * Plans an A* path for each agent that has a valid goal.
     * If no path is found, the agent's path remains empty.
     */
    void planPaths();

    /**
     * Overwrites each agent's path cells in the map with the agent's startVal.
     * Only applies to agents that actually have a path (path not empty).
     */
    void markPathsOnMap();

    /**
     * Moves each agent forward by one cell on its path if the next cell
     * is not occupied by another agent. If collision would occur, the agent
     * waits this turn.
     */
    void step();

    /**
     * @return True if every agent with a path has reached the end
     *         (or if no path was assigned, they're effectively idle).
     */
    bool allArrived() const;

    /**
     * Utility method for debugging: prints agent positions, goals, and path states.
     */
    void printAgents() const;

private:
    Map& map;
    std::vector<Agent> agents;                // Our agent list
    std::vector<std::pair<int,int>> goalCells; // Discovered goal cells

    // For measuring distance in 'assignGoals()' (Manhattan or Euclidean)
    double computeDistance(int r1, int c1, int r2, int c2);

    // Checks if the given cell is currently occupied by an agent
    bool isOccupied(int row, int col) const;
};