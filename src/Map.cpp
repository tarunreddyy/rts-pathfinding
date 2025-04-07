/******************************************************************************
 * File: Map.cpp
 *
 * Description:
 *   Implementation of the Map class. This class loads a 2D grid from a JSON file
 *   by using a custom JsonParser. It assumes the JSON has a "layers[0].data" array
 *   containing the tile values. The Map stores these values in a flattened
 *   std::vector<int>, providing getters and setters for cell access.
 *
 * Usage:
 *   1) Create a Map object.
 *   2) Call loadFromJson(...) with the path to the JSON file.
 *   3) Access the grid dimensions via getWidth() / getHeight().
 *   4) Access or modify cell values via getCell(...) / setCell(...).
 *
 * Author: Tarun Trilokesh
 * Date:   2025-06-04
 ******************************************************************************/

#include "Map.h"
#include "JsonParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

/**
 * * @brief Loads map data from a JSON file.
 * *
 * This function attempts to open the specified JSON file, parse it using
 * the JsonParser class, and store the resulting grid data in the Map object.
 * The JSON file is expected to contain a "layers[0].data" array of integers,
 * which will populate the grid. The function checks if the data size is a
 * perfect square and sets the width and height accordingly.
 * 
 * @param filePath Path to the JSON file.
 * @return True if loading and parsing succeed; false otherwise.
 */
bool Map::loadFromJson(const std::string& filePath)
{
    // Attempt to open the JSON file
    std::ifstream fin(filePath);
    if (!fin) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }

    // Read the entire file contents into a string
    std::string jsonContent(
        (std::istreambuf_iterator<char>(fin)),
         std::istreambuf_iterator<char>()
    );

    // Use the custom JsonParser to extract the data array
    JsonParser parser;
    if (!parser.parseJson(jsonContent)) {
        std::cerr << "Error parsing JSON data." << std::endl;
        return false;
    }

    // Store the parsed data in our vector
    // Assuming a square map (width == height)
    auto dataVector = parser.getGridData();
    int dataCount = static_cast<int>(dataVector.size());
    int dim = static_cast<int>(std::sqrt(dataCount));

    // If dataCount is not a perfect square, reject the map
    if (dim * dim != dataCount) {
        std::cerr << "Invalid grid data size: " << dataCount << std::endl;
        return false;
    }

    width = dim;
    height = dim;
    gridData = dataVector; // Flattened grid

    return true;
}

/**
 * @brief Retrieves the value at row r, column c.
 * 
 * This function returns the double value stored at the specified grid position.
 * It throws an std::out_of_range exception if the coordinates are outside the grid.
 * 
 * @param r Row index (0-based).
 * @param c Column index (0-based).
 * @return The double value stored at that grid position.
 * @throws std::out_of_range if (r,c) is outside the grid.
 */
double Map::getCell(int r, int c) const
{
    // Returns the integer stored at row r, column c.
    // Throws an exception if out of bounds.
    if (r < 0 || r >= height || c < 0 || c >= width) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return gridData[r * width + c];
}

/**
 * @brief Sets the grid value at (r, c) to the specified integer.
 * 
 * This function modifies the value at the specified grid position. It throws
 * an std::out_of_range exception if the coordinates are outside the grid.
 * 
 * @param r Row index (0-based).
 * @param c Column index (0-based).
 * @param value The new integer value to store.
 * @throws std::out_of_range if (r,c) is outside the grid.
 */
void Map::setCell(int r, int c, double value)
{
    // Modifies the value at row r, column c.
    // Throws an exception if out of bounds.
    if (r < 0 || r >= height || c < 0 || c >= width) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    gridData[r * width + c] = value;
}

/**
 * @brief Finds all cells in the grid that match the specified value.
 * 
 * This function iterates through the grid and collects the coordinates of
 * all cells that match the target value. It returns a vector of (row, column)
 * pairs where the value matches targetValue.
 * 
 * @param targetValue The value to search for.
 * @return A vector of (row, column) pairs where the value matches targetValue.
 */
std::vector<std::pair<int,int>> Map::findCellsByValue(double targetValue) const
{
    // Tiny epsilon, in case of floating error
    const double EPS = 1e-6;
    std::vector<std::pair<int,int>> positions;
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            double cellVal = getCell(r, c);
            if (std::fabs(cellVal - targetValue) < EPS) {
                positions.push_back({r, c});
            }
        }
    }
    return positions;
}