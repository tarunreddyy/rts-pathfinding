/******************************************************************************
 * File: MultiUnitCoordinator.cpp
 *
 * Description:
 *   Implementation of the multi-agent logic:
 *   - Scans for starts (0.5,0.6,0.9) and goals (8.1,8.4,8.13).
 *   - Assigns each agent to the nearest goal.
 *   - Uses A* to plan paths.
 *   - Steps agents 1 cell at a time, avoiding collisions.
 *
 * Author: Tarun Trilokesh
 * Date:   2025-06-04
 ******************************************************************************/

#include "MultiUnitCoordinator.h"
#include <limits>
#include <iostream>
#include <cmath>

/******************************************************************************
 * @brief Constructor for MultiUnitCoordinator.
 * 
 * This constructor initializes the MultiUnitCoordinator with a reference to a Map object.
 * It sets up the internal data structures for managing agents and their goals.
 * 
 * @param mapRef Reference to the Map object that this coordinator will manage.
 */
MultiUnitCoordinator::MultiUnitCoordinator(Map& mapRef)
    : map(mapRef)
{
}

/******************************************************************************
 * @brief Finds all agent start positions and goal positions in the map.
 * 
 * This function scans the map for specific start values (0.5, 0.6, 0.9) and
 * goal values (8.1, 8.4, 8.13). It populates the agents vector with Agent
 * objects representing the start positions and the goalCells vector
 * with pairs of coordinates representing the goal positions.
 * 
 * The function also prints the number of agents and goals found.
 */
void MultiUnitCoordinator::findStartsAndGoals()
{
    // Potential agent start values
    static const double START_VALUES[] = {0.5, 0.6, 0.9};
    // Potential goal values
    static const double GOAL_VALUES[]  = {8.1, 8.4, 8.13};

    int agentIdCount = 0;

    // Gather all agent starts
    for (double sv : START_VALUES) {
        auto positions = map.findCellsByValue(sv);
        for (auto &pos : positions) {
            Agent ag;
            ag.id = agentIdCount++;
            ag.startVal = sv;     // store the start tile value
            ag.row = pos.first;
            ag.col = pos.second;
            ag.goalRow = -1;
            ag.goalCol = -1;
            ag.pathIndex = 0;
            agents.push_back(ag);
        }
    }

    // Gather all goals
    for (double gv : GOAL_VALUES) {
        auto positions = map.findCellsByValue(gv);
        for (auto &pos : positions) {
            goalCells.push_back(pos);
        }
    }

    std::cout << "Found " << agents.size() << " agent(s) and "
              << goalCells.size() << " goal(s).\n";
}

/*******************************************************************************
 * @brief Assigns each agent the nearest goal.
 * 
 * This function iterates through the list of agents and assigns each one
 * the nearest goal based on Manhattan distance. The same goal can be assigned
 * to multiple agents.
 * 
 * @note If no goals are available, the agent's goalRow and goalCol are set to -1.
 *       This indicates that the agent has no assigned goal.
 */
void MultiUnitCoordinator::assignGoals()
{
    if (agents.empty() || goalCells.empty()) {
        std::cout << "No agents or no goals to assign.\n";
        return;
    }

    // If the number of agents == number of goals, assign 1:1 in order
    if (agents.size() == goalCells.size()) {
        std::cout << "Assigning distinct goals because agent count = goal count.\n";
        size_t n = agents.size();
        // For each agent i, assign goal i
        for (size_t i = 0; i < n; ++i) {
            auto &agent = agents[i];
            auto &gcell = goalCells[i];
            agent.goalRow = gcell.first;
            agent.goalCol = gcell.second;
            std::cout << "Agent " << agent.id
                      << " assigned goal (" << agent.goalRow
                      << "," << agent.goalCol << ")\n";
        }
    }
    else {
        // Otherwise, use nearest goal logic
        std::cout << "Assigning goals by nearest-distance (some goals may be shared).\n";

        // Each agent picks the single nearest goal
        for (auto &agent : agents) {
            double bestDist = std::numeric_limits<double>::infinity();
            int bestGoalIdx = -1;

            for (int i = 0; i < (int)goalCells.size(); ++i) {
                auto &gcell = goalCells[i];
                double dist = computeDistance(agent.row, agent.col,
                                              gcell.first, gcell.second);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestGoalIdx = i;
                }
            }

            if (bestGoalIdx >= 0) {
                auto &gcell = goalCells[bestGoalIdx];
                agent.goalRow = gcell.first;
                agent.goalCol = gcell.second;
                std::cout << "Agent " << agent.id
                          << " assigned goal ("
                          << agent.goalRow << "," << agent.goalCol << ")\n";
            } else {
                agent.goalRow = -1;
                agent.goalCol = -1;
                std::cout << "Agent " << agent.id
                          << " found no available goal.\n";
            }
        }
    }
}


/*******************************************************************************
 * @brief Plans paths for each agent using A* algorithm.
 * 
 * This function iterates through the list of agents and attempts to find a
 * path from each agent's current position to its assigned goal using the
 * A* algorithm. If a path is found, it is stored in the agent's path member.
 * If no path is found, the agent's path remains empty.
 */
void MultiUnitCoordinator::planPaths()
{
    for (auto &agent : agents) {
        if (agent.goalRow < 0 || agent.goalCol < 0) {
            // No goal => skip
            continue;
        }

        // Attempt A* path
        auto path = Pathfinding::aStar(map,
                                       agent.row, agent.col,
                                       agent.goalRow, agent.goalCol);
        if (path.empty()) {
            std::cout << "Agent " << agent.id << " => No path found.\n";
        } else {
            agent.path = path;
            agent.pathIndex = 0;
            std::cout << "Agent " << agent.id
                      << " path length: " << path.size() << "\n";
        }
    }
}

/*******************************************************************************
 * @brief Marks each agent's path on the map with its start value.
 * 
 * This function iterates through the list of agents and marks their paths
 * on the map using their respective start values. Agents that have no path
 * are skipped.
 */
void MultiUnitCoordinator::markPathsOnMap()
{
    // Overwrite each agent's path cells with its startVal
    // Agents that found no path => skip
    for (auto &agent : agents) {
        if (agent.path.empty()) {
            continue;
        }
        // Mark the entire path
        for (size_t i = 0; i < agent.path.size() - 1; ++i) {
            auto &cell = agent.path[i];
            map.setCell(cell.first, cell.second, agent.startVal);
        }
    }
    std::cout << "Marked each agent's path in the map.\n";
}

/*******************************************************************************
 * @brief Moves each agent one step along its path if the next cell is free.
 * 
 * This function iterates through the list of agents and checks if the next
 * cell in their path is occupied. If it is free, the agent moves to that cell.
 * If the path is empty or the agent has reached its goal, it does not move.
 */
void MultiUnitCoordinator::step()
{
    // Move each agent 1 step if the next cell is free
    for (auto &agent : agents) {
        if (agent.path.empty() || agent.pathIndex >= (int)agent.path.size() - 1) {
            // No path or done traveling
            continue;
        }
        // Next step
        auto [nr, nc] = agent.path[agent.pathIndex + 1];
        // Check if occupied
        if (!isOccupied(nr, nc)) {
            // Move agent
            agent.row = nr;
            agent.col = nc;
            agent.pathIndex++;
        }
        // else wait this turn
    }
}

/*******************************************************************************
 * @brief Checks if all agents have reached their goals.
 * 
 * This function iterates through the list of agents and checks if each agent
 * has either reached its goal or has no assigned goal. If any agent is still
 * on its path, the function returns false.
 *
 * @return True if all agents have reached their goals or have no paths; false otherwise.
 */
bool MultiUnitCoordinator::allArrived() const
{
    for (auto &agent : agents) {
        // If path not empty and we haven't reached the last cell
        if (!agent.path.empty() &&
            agent.pathIndex < (int)agent.path.size() - 1) {
            return false;
        }
    }
    return true;
}

/*******************************************************************************
 * @brief Prints the current state of all agents.
 * 
 * This function iterates through the list of agents and prints their
 * current position, assigned goal (if any), and path index.
 *
 * @param agents A vector of Agent objects representing the agents.
 */
void MultiUnitCoordinator::printAgents() const
{
    for (auto &agent : agents) {
        std::cout << "Agent " << agent.id
                  << " startVal=" << agent.startVal
                  << " at (" << agent.row << "," << agent.col << ")";
        if (agent.goalRow >= 0) {
            std::cout << " => Goal(" << agent.goalRow
                      << "," << agent.goalCol << ")";
        } else {
            std::cout << " => NoGoal";
        }
        std::cout << " [pathIndex=" << agent.pathIndex
                  << "/" << (agent.path.size() - 1) << "]\n";
    }
    std::cout << std::endl;
}

/*******************************************************************************
 * @brief Computes the Manhattan distance between two cells.
 * 
 * This function calculates the Manhattan distance between two cells
 * specified by their row and column indices. The Manhattan distance is the
 * sum of the absolute differences of their coordinates.
 * 
 * @param r1 Row index of the first cell.
 * @param c1 Column index of the first cell.
 * @param r2 Row index of the second cell.
 * @param c2 Column index of the second cell.
 * @return The Manhattan distance between the two cells.
 */
double MultiUnitCoordinator::computeDistance(int r1, int c1, int r2, int c2)
{
    // Simple Manhattan distance
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

/*******************************************************************************
 * @brief Checks if a cell is occupied by any agent.
 * 
 * This function iterates through the list of agents and checks if any agent
 * is currently occupying the specified cell (row, col).
 * 
 * @param row Row index of the cell to check.
 * @param col Column index of the cell to check.
 * @return True if occupied, false otherwise.
 */
bool MultiUnitCoordinator::isOccupied(int row, int col) const
{
    for (auto &agent : agents) {
        if (agent.row == row && agent.col == col) {
            return true;
        }
    }
    return false;
}