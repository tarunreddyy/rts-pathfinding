/******************************************************************************
 * File: JsonParser.h
 *
 * Description:
 * Parses the JSON content, looking for "layers" and then "data" in
 * the first layer. Extracts numeric values (including floating-point)
 * and stores them in linearGridArray.
 *
 * @param jsonData A string containing the entire JSON content.
 * @return True if parsing succeeded and data was extracted; false otherwise.
 *
 * Author: Tarun Trilokesh
 * Date:   2025-04-04
 *****************************************************************************/

#pragma once

#include <string>
#include <vector>
 
class JsonParser {
public:
    /**
     * Parses the JSON content, looking for "layers" and then "data" in
     * the first layer. Extracts numeric values (float/double truncated
     * to int) and stores them in linearGridArray.
     *
     * @param jsonData A string containing the entire JSON content.
     * @return True if parsing succeeded and data was extracted; false otherwise.
     */
    bool parseJson(const std::string& jsonData);
 
    /**
     * Returns a copy of the flattened grid data extracted from the JSON file.
     * Each entry corresponds to one tile on the map.
     *
     * @return A vector of integers (the flattened map data).
     */
    std::vector<double> getGridData() const { return linearGridArray; }
 
private:
    // Stores the flattened map data from "layers[0].data"
    std::vector<double> linearGridArray;
};

 