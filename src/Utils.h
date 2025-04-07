#pragma once

/******************************************************************************
 * File:    Utils.h
 *
 * Overview:
 *   This header file provides utility functions for:
 *     1) Generating a JSON string based on the updated Map data (generateJsonOutput).
 *
 * Author:  Tarun Trilokesh
 * Date:    2025-06-04
 ******************************************************************************/

#include <string>
#include <vector>
#include "Map.h"


/**
 * @brief Generate a JSON string based on the map data.
 * 
 * This function constructs a JSON-like string that represents the map's
 * grid data. The output format is similar to what Tiled uses, with a
 * "layers" array and a "tilesets" section.
 * 
 * @param map   Reference to the Map object.
 * @param originalJson  Path to the original JSON file (not used in this function).
 * @return A string containing the generated JSON output.
 */
inline std::string generateJsonOutput(const Map& map, 
                                      const std::string& /*originalJson*/) 
{
    // Get width and height of the map
    int w = map.getWidth();
    int h = map.getHeight();

    // Rebuild the data array as a comma-separated list
    // Reserve space to optimize string appends
    std::string dataArray;
    dataArray.reserve((w * h * 2) + 100); 
    for (int i = 0; i < w * h; ++i) {
        dataArray += std::to_string(map.getCell(i / w, i % w));
        if (i < (w * h - 1)) {
            dataArray += ", ";
        }
    }

    // Construct the output in a simple Tiled-like JSON format
    std::string out = R"({
  "layers": [
    {
      "name": "world",
      "tileset": "MapEditor Tileset_woodland.png",
      "data": [
)";

    // Insert our generated data array into the JSON
    out += dataArray;
    out += R"(
      ]
    }
  ],
  "tilesets": [
    {
      "name": "MapEditor Tileset_woodland.png",
      "image": "MapEditor Tileset_woodland.png",
      "imagewidth": 512,
      "imageheight": 512,
      "tilewidth": 32,
      "tileheight": 32
    }
  ],
  "canvas": {
    "width": 1024,
    "height": 1024
  }
}
)";

    return out;
}
