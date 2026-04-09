#pragma once

#include <string>
#include <vector>

#include "SmartPointers.h"

struct VideoStream
{
  int width;
  int height;
};

struct AudioStream
{
  int sample_rate;
  int channels;
};

struct MediaProperties
{
  std::string container_name;
  std::vector<VideoStream> video_streams;
  std::vector<AudioStream> audio_streams;
};

class MediaAnalyzer
{
public:
  MediaAnalyzer(const std::string& file_path);
  MediaProperties analyze();

private:
  std::string _file_path;
};