#include "ProbeCommand.h"

#include <fmt/core.h>

ProbeCommand::ProbeCommand(const std::string& probe_input, const std::string& filename)
  : _probe_input(probe_input)
  , _filename(filename)
{
}

void ProbeCommand::execute()
{
  MediaAnalyzer analyzer(_probe_input);
  MediaProperties properties = analyzer.analyze();
  print_media_properties(properties);
}

void ProbeCommand::print_media_properties(const MediaProperties& properties)
{
  fmt::print("Container: {}\n", properties.container_name);

  for(size_t i = 0; i < properties.video_streams.size(); ++i) {
    const auto& video_stream = properties.video_streams[i];
    fmt::print("Video Stream {}: {}x{}\n", i, video_stream.width, video_stream.height);
  }

  for(size_t i = 0; i < properties.audio_streams.size(); ++i) {
    const auto& audio_stream = properties.audio_streams[i];
    fmt::print(
      "Audio Stream {}: {} Hz, {} channels\n",
      i,
      audio_stream.sample_rate,
      audio_stream.channels
    );
  }
}