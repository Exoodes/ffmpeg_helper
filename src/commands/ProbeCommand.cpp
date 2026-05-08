#include "ProbeCommand.h"

#include <fmt/core.h>

// -------------------------------------------------------------------------------------------------
ProbeCommand::ProbeCommand(const std::string& probe_input, const std::string& filename)
  : _probe_input(probe_input)
  , _filename(filename)
{
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::execute()
{
  MediaAnalyzer analyzer(_probe_input);
  MediaProperties properties = analyzer.analyze();
  print_media_properties(properties);
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_tags(
  const std::map<std::string, std::string>& tags,
  const std::string& indent
)
{
  if(!tags.empty()) {
    fmt::print("{}Metadata Tags:\n", indent);
    for(const auto& tag : tags) {
      fmt::print("{}  {}: {}\n", indent, tag.first, tag.second);
    }
  }
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_chapters(const std::vector<Chapter>& chapters)
{
  if(!chapters.empty()) {
    fmt::print("Chapters:\n");
    for(const auto& chapter : chapters) {
      fmt::print(
        "  Chapter {}: Start: {:.3f}s, End: {:.3f}s\n",
        chapter.id,
        chapter.start_time,
        chapter.end_time
      );
      print_tags(chapter.tags, "    ");
    }
  }
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_media_properties(const MediaProperties& properties)
{
  fmt::print("Container: {} ({})\n", properties.container_name, properties.container_long_name);
  fmt::print("Start Time: {:.3f} seconds\n", properties.start_time_seconds);
  fmt::print("Duration: {:.2f} seconds\n", properties.duration_seconds);
  fmt::print("Overall Bit Rate: {} bps\n", properties.overall_bit_rate);
  fmt::print("File Size: {} bytes\n", properties.file_size_bytes);

  print_tags(properties.tags, "");
  fmt::print("\n");

  print_chapters(properties.chapters);
  if(!properties.chapters.empty()) {
    fmt::print("\n");
  }

  for(const auto& video_stream : properties.video_streams) {
    print_video_stream_info(video_stream);
  }

  for(const auto& audio_stream : properties.audio_streams) {
    print_audio_stream_info(audio_stream);
  }
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_video_stream_info(const VideoStream& video_stream)
{
  fmt::print(
    "Video Stream {}: {}x{} @ {}/{} fps | {} | {} | Profile: {} | Bit Rate: {} bps\n",
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
  fmt::print("  Time Base: {}/{}\n", video_stream.time_base_num, video_stream.time_base_den);
  fmt::print("  Start Time: {:.3f}s\n", video_stream.start_time);
  fmt::print("  Total Frames: {}\n", video_stream.total_frames);
  fmt::print(
    "  Color Space: {} | Primaries: {} | TRC: {} | HDR: {}\n",
    video_stream.color_space.empty() ? "Unknown" : video_stream.color_space,
    video_stream.color_primaries.empty() ? "Unknown" : video_stream.color_primaries,
    video_stream.color_trc.empty() ? "Unknown" : video_stream.color_trc,
    video_stream.is_hdr ? "Yes" : "No"
  );
  fmt::print("  Field Order: {}\n", video_stream.field_order);
  print_tags(video_stream.tags, "  ");
  fmt::print("\n");
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_audio_stream_info(const AudioStream& audio_stream)
{
  fmt::print(
    "Audio Stream {}: {} Hz | {} channels | {} | {} | {} | Profile: {} | Bit Rate: {} bps\n",
    audio_stream.index,
    audio_stream.sample_rate,
    audio_stream.channels,
    audio_stream.channel_layout,
    audio_stream.sample_format,
    audio_stream.codec_name,
    audio_stream.profile,
    audio_stream.bit_rate
  );
  fmt::print("  Time Base: {}/{}\n", audio_stream.time_base_num, audio_stream.time_base_den);
  fmt::print("  Start Time: {:.3f}s\n", audio_stream.start_time);
  fmt::print("  Total Frames: {}\n", audio_stream.total_frames);
  print_tags(audio_stream.tags, "  ");
  fmt::print("\n");
}