#include "Utility.h"

#include <fmt/core.h>

#include "FFmpegHeaders.h"

std::string get_av_error_string(int errnum)
{
  char errbuf[AV_ERROR_MAX_STRING_SIZE];
  av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, errnum);
  return std::string(errbuf);
}

bool logAVERROR(int averr)
{
  if(averr < 0) {
    fmt::print("FFmpeg error: {}\n", get_av_error_string(averr));
    return true;
  }
  return false;
}

std::string getStringFromCharArray(const char* char_array, std::string default_value)
{
  if(char_array) {
    return std::string(char_array);
  }

  return default_value;
}