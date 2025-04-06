/******************************************************************************
 * File: JsonParser.cpp
 *
 * Description:
 *   A manual (naive) JSON parser that finds the "layers" array,
 *   then within it finds the first "data" array, and extracts each numeric
 *   entry. These values get stored in 'linearGridArray'.
 *
 *   NOTE: This approach is fragile and does not handle nested objects or
 *   escaped quotes, among other complexities. It is meant only for
 *   demonstration and for maps with a structure similar to the sample given.
 *
 * Usage:
 *   1) Include "JsonParser.h" in code.
 *   2) Create a JsonParser object, call parseJson(jsonString),
 *      and then retrieve the data via getGridData().
 *
 * Author: Tarun Trilokesh
 * Date:   2025-04-04
 *
 ******************************************************************************/

#include "JsonParser.h"
#include <iostream>
#include <cctype>   // for std::isspace
#include <cstring>  // for std::strncmp
#include <cstdlib>  // for std::atof

bool JsonParser::parseJson(const std::string& jsonData)
{
    // Convert std::string to C-style string for pointer-based parsing
    const char* pos = jsonData.c_str();

    bool foundLayers = false;  // Whether we've encountered "layers"
    bool readingData = false;  // Whether we're currently extracting "data" array values

    // Parse until the end of the string ('\0')
    while(*pos != '\0') 
    {
        // Skip any leading whitespace (spaces, tabs, newlines)
        while (std::isspace((unsigned char)*pos)) {
            ++pos;
        }

        // Locate the "layers" array by searching for the literal "\"layers\":"
        if (!foundLayers && std::strncmp(pos, "\"layers\":", 8) == 0) {
            foundLayers = true;
            pos += 8; // Move the pointer past "\"layers\":"

            // Move forward until we find '[' or end of string
            while (*pos != '[' && *pos != '\0') {
               ++pos;
            }
        }

        // Once we've found "layers", look for the "\"data\":"
        if (foundLayers && std::strncmp(pos, "\"data\":", 6) == 0) {
            // Move pointer past "\"data\":"
            pos += 6;

            // Move forward until we find the '[' that starts the "data" array
            while (*pos != '[' && *pos != '\0') {
                ++pos;
            }

            // If we find '[', we are now ready to read the data
            if (*pos == '[') {
                ++pos;         // Move past '['
                readingData = true;  // Indicate we're reading the tile data
            }
        }

        // If we are reading the "data" array, parse numbers until ']' or '\0'
        if (readingData) {
            // Keep reading tokens until we reach ']' or end of string
            while (*pos != ']' && *pos != '\0') {
                // Skip any whitespace between numbers
                while (std::isspace((unsigned char)*pos)) {
                    ++pos;
                }

                // If we hit ']' or end of string, break from data reading
                if (*pos == ']' || *pos == '\0') {
                    readingData = false;
                    break;
                }

                // Temporary buffer to capture one numeric value
                char numberBuffer[64];
                int nbIndex = 0;

                // Read the numeric characters (up to ',' or ']' or '\0')
                while (*pos != ',' && *pos != ']' && *pos != '\0') {
                    if (std::isdigit(*pos) || *pos == '.' || *pos == '-' || *pos == '+') {
                        // Only allow valid numeric characters
                        if (nbIndex < (int)sizeof(numberBuffer) - 1) {
                            numberBuffer[nbIndex++] = *pos;
                        }
                    } else if (!std::isspace((unsigned char)*pos)) {
                        // Log an error for invalid characters (excluding whitespace)
                        std::cerr << "Invalid character in numeric data: " << *pos << "\n";
                        return false; // Exit on invalid character
                    }
                    ++pos;
                }

                // Null-terminate the buffer to make a proper C-string
                numberBuffer[nbIndex] = '\0';

                // Convert this numeric string to a double
                if (nbIndex > 0) {
                    try {
                        double numValue = std::atof(numberBuffer);
                        // Store the number in the linearGridArray
                        linearGridArray.push_back(numValue);
                    } catch (...) {
                        std::cerr << "Error converting number: " << numberBuffer << "\n";
                        return false; // Exit on conversion error
                    }
                }

                // If we ended on a comma, skip it and move to next number
                if (*pos == ',') {
                    ++pos;
                }
            }

            // If we see ']' here, we've finished reading the "data" array
            if (*pos == ']') {
                readingData = false;
            }
        }

        // Move past the current character if it's not '\0'
        if (*pos != '\0') {
            ++pos;
        }
    }

    // Did we successfully extract any numbers?
    if (!linearGridArray.empty()) {
        std::cout << "Parsed grid data successfully!\n";
        return true;
    } else {
        std::cerr << "Failed to parse grid data.\n";
        return false;
    }
} // end of parseJson