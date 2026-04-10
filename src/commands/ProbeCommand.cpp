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
  fmt::print("Container: {} ({})\n", properties.container_name, properties.container_long_name);
  fmt::print("Duration: {:.2f} seconds\n", properties.duration_seconds);
  fmt::print("Overall Bit Rate: {} bps\n", properties.overall_bit_rate);
  fmt::print("File Size: {} bytes\n", properties.file_size_bytes);

  for(size_t i = 0; i < properties.video_streams.size(); ++i) {
    const auto& video_stream = properties.video_streams[i];
    print_video_stream_info(video_stream);
  }

  for(size_t i = 0; i < properties.audio_streams.size(); ++i) {
    const auto& audio_stream = properties.audio_streams[i];
    print_audio_stream_info(audio_stream);
  }
}

void ProbeCommand::print_video_stream_info(const VideoStream& video_stream)
{
  fmt::print(
    "Video Stream {}: {}x{}, {}/{} fps, {}, {}, profile: {}, bit_rate: {} bps\n",
    video_stream.index,
    video_stream.width,
    video_stream.height,
    video_stream.fps_num,
    video_stream.fps_den,
    video_stream.codec_name,
    video_stream.pixel_format,
    video_stream.profile,
    video_stream.bit_rate
  );
}

void ProbeCommand::print_audio_stream_info(const AudioStream& audio_stream)
{
  fmt::print(
    "Audio Stream {}: {} Hz, {} channels, {}, {}, {}, profile: {}, bit_rate: {} bps\n",
    audio_stream.index,
    audio_stream.sample_rate,
    audio_stream.channels,
    audio_stream.channel_layout,
    audio_stream.sample_format,
    audio_stream.codec_name,
    audio_stream.profile,
    audio_stream.bit_rate
  );
}