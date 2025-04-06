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
 * Date:   2025-03-04
 ******************************************************************************/

#include "Map.h"
#include "JsonParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

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
    //    - We assume a square map (width == height)
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

double Map::getCell(int r, int c) const
{
    // Returns the integer stored at row r, column c.
    // Throws an exception if out of bounds.
    if (r < 0 || r >= height || c < 0 || c >= width) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return gridData[r * width + c];
}

void Map::setCell(int r, int c, double value)
{
    // Modifies the value at row r, column c.
    // Throws an exception if out of bounds.
    if (r < 0 || r >= height || c < 0 || c >= width) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    gridData[r * width + c] = value;
} 