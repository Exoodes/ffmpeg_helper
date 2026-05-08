#pragma once

#include <map>
#include <string>

#include <media/MediaAnalyzer.h>

#include "ICommand.h"

class ProbeCommand : public ICommand
{
public:
  ProbeCommand(const std::string& probe_input, const std::string& filename);
  void execute() override;

private:
  void print_media_properties(const MediaProperties& properties);
  void print_video_stream_info(const VideoStream& video_stream);
  void print_audio_stream_info(const AudioStream& audio_stream);
  void print_tags(const std::map<std::string, std::string>& tags, const std::string& indent);
  void print_chapters(const std::vector<Chapter>& chapters);

  std::string _probe_input;
  std::string _filename;
};