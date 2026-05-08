#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "MediaStructs.hpp"
#include "SmartPointers.h"

// -------------------------------------------------------------------------------------------------
class BitrateHistogram
{
public:
  std::map<uint32_t, uint32_t> data;
  uint32_t time_interval_ms;
};

// -------------------------------------------------------------------------------------------------
class BitrateScanner
{
public:
  BitrateScanner(uint32_t time_interval_ms, AVFormatContextPtr ctx);
  std::map<uint32_t, BitrateHistogram> scan();

private:
  uint32_t _time_interval_ms;
  AVFormatContextPtr _ctx;
};

// -------------------------------------------------------------------------------------------------
class MediaAnalyzer
{
public:
  MediaAnalyzer(const std::string& file_path);
  MediaProperties analyze();
  std::map<uint32_t, BitrateHistogram> analyze_bitrate(uint32_t time_interval_ms);

private:
  AVFormatContextPtr open_input(const std::string& file_path);

  VideoStream extract_video_stream_info(AVStream* stream);
  AudioStream extract_audio_stream_info(AVStream* stream);
  std::map<std::string, std::string> extract_metadata(AVDictionary* dict);

  std::string _file_path;
};