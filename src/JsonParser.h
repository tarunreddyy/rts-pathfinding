/******************************************************************************
 * File: JsonParser.h
 *
 * Description:
 *   A minimal, naive JSON parser designed to extract the tile data from
 *   the "data" array in the first layer of a map file (e.g., a RiskyLab
 *   tilemap JSON). This parser is not general-purpose JSON; it focuses on
 *   a specific structure and may fail if the JSON format deviates.
 *
 * Usage:
 *   1) Create a JsonParser instance.
 *   2) Call parseJson(...) with the raw JSON string from the file.
 *   3) Retrieve the parsed grid array via getGridData().
 *
 * Author: Tarun Trilokesh
 * Date:   2025-03-04
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
    std::vector<int> getGridData() const { return linearGridArray; }
 
private:
    // Stores the flattened map data from "layers[0].data"
    std::vector<int> linearGridArray;
};

 