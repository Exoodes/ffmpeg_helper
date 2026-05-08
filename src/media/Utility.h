#pragma once

#include <string>

/**
   * Log the converted string to the log as an error.
   * 
   * @param averror The return value of an FFmpeg API function. If the value is an AVERROR, it will
   *                be logged.
   * @returns true if the return value was an error. Otherwise returns false if the return value was
   *          an expected condition.
   */
bool logAVERROR(int averr);

/**
 * @brief Converts a C-style character array to a std::string.
 * 
 * Safely converts a null-terminated character array pointer to a std::string object.
 * If the input pointer is nullptr or invalid, returns the provided default value.
 * 
 * @param char_array Pointer to a null-terminated C-style string to convert.
 *                   Can be nullptr, in which case default_value is returned.
 * @param default_value The string to return if char_array is nullptr or empty.
 *                      Defaults to an empty string if not specified.
 * 
 * @return std::string The converted string, or default_value if char_array is nullptr.
 */
std::string getStringFromCharArray(const char* char_array, std::string default_value);

inline std::string getStringFromCharArray(const char* char_array)
{
  return getStringFromCharArray(char_array, "");
}