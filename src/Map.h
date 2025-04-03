/******************************************************************************
 * File: Map.h
 *
 * Description:
 *   This header defines the Map class, which encapsulates a 2D grid loaded
 *   from a JSON file (via a custom JsonParser). It provides getters for the
 *   grid dimensions and cell values, as well as a setter to modify cell data.
 *
 * Usage:
 *   1) Create a Map object.
 *   2) Call loadFromJson(...) with the path to your JSON file.
 *   3) Access dimensions using getWidth() / getHeight().
 *   4) Access or modify cell values using getCell(...) / setCell(...).
 *
 * Author: Tarun Trilokesh
 * Date:   2025-03-04
 ******************************************************************************/

#pragma once

#include <string>
#include <vector>

class Map {
public:
    /**
     * Loads map data from a JSON file. The file is expected to have a
     * "layers[0].data" array of integers, which will populate the grid.
     *
     * @param filePath Path to the JSON file.
     * @return True if loading and parsing succeed; false otherwise.
     */
    bool loadFromJson(const std::string& filePath);

    /**
     * @return The width of the grid (number of columns).
     */
    int getWidth() const { return width; }

    /**
     * @return The height of the grid (number of rows).
     */
    int getHeight() const { return height; }

    /**
     * Retrieves the value at row r, column c.
     *
     * @param r Row index (0-based).
     * @param c Column index (0-based).
     * @return The integer value stored at that grid position.
     * @throws std::out_of_range if (r,c) is outside the grid.
     */
    int getCell(int r, int c) const;

    /**
     * Sets the grid value at (r, c) to the specified integer.
     *
     * @param r Row index (0-based).
     * @param c Column index (0-based).
     * @param value The new integer value to store.
     * @throws std::out_of_range if (r,c) is outside the grid.
     */
    void setCell(int r, int c, int value);

private:
    int width = 0;               ///< Number of columns
    int height = 0;              ///< Number of rows
    std::vector<int> gridData;   ///< Flattened grid data (size = width*height)
};