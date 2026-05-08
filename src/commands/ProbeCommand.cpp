#include "ProbeCommand.h"

#include <fmt/core.h>

constexpr auto RESET = "\033[0m";
constexpr auto BOLD = "\033[1m";
constexpr auto HEADER = "\033[1;36m"; // Bold Cyan
constexpr auto STREAM = "\033[1;32m"; // Bold Green
constexpr auto KEY = "\033[33m";      // Yellow
constexpr auto DIM = "\033[2m";       // Dim/Faint

static std::string format_bytes(int64_t bytes)
{
  const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
  int i = 0;
  double dblBytes = static_cast<double>(bytes);
  while(dblBytes >= 1024.0 && i < 4) {
    dblBytes /= 1024.0;
    i++;
  }
  return fmt::format("{:.2f} {}", dblBytes, suffixes[i]);
}

static std::string format_bitrate(int64_t bitrate)
{
  if(bitrate == 0) {
    return "N/A";
  }
  return fmt::format("{:.0f} kbps", static_cast<double>(bitrate) / 1000.0);
}

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
    fmt::print("{}{}[Metadata Tags]{}\n", indent, DIM, RESET);
    for(const auto& tag : tags) {
      fmt::print("{}  {}{:<15}{}: {}\n", indent, KEY, tag.first, RESET, tag.second);
    }
  }
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_chapters(const std::vector<Chapter>& chapters)
{
  if(!chapters.empty()) {
    fmt::print("\n{}{}[Chapters]{}\n", DIM, BOLD, RESET);
    for(const auto& chapter : chapters) {
      fmt::print(
        "  {}Chapter {:02d}{}: Start: {:.3f}s, End: {:.3f}s\n",
        KEY,
        chapter.id,
        RESET,
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
  fmt::print("\n{}{}=== Media Properties ==={}\n", HEADER, BOLD, RESET);
  fmt::print(
    "{}{:<15}{}: {} ({})\n",
    KEY,
    "Container",
    RESET,
    properties.container_name,
    properties.container_long_name
  );
  fmt::print(
    "{}{:<15}{}: {:.3f} seconds\n",
    KEY,
    "Start Time",
    RESET,
    properties.start_time_seconds
  );
  fmt::print("{}{:<15}{}: {:.2f} seconds\n", KEY, "Duration", RESET, properties.duration_seconds);
  fmt::print(
    "{}{:<15}{}: {}\n",
    KEY,
    "Bit Rate",
    RESET,
    format_bitrate(properties.overall_bit_rate)
  );
  fmt::print("{}{:<15}{}: {}\n", KEY, "File Size", RESET, format_bytes(properties.file_size_bytes));
  fmt::print("\n");

  print_tags(properties.tags, "");
  print_chapters(properties.chapters);
  fmt::print("\n");

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
  fmt::print("{}{}--- Video Stream #{} ---{}\n", STREAM, BOLD, video_stream.index, RESET);
  fmt::print(
    "  {}{:<15}{}: {} (Profile: {})\n",
    KEY,
    "Codec",
    RESET,
    video_stream.codec_name,
    video_stream.profile
  );
  fmt::print(
    "  {}{:<15}{}: {}x{} \n",
    KEY,
    "Resolution",
    RESET,
    video_stream.width,
    video_stream.height
  );
  fmt::print(
    "  {}{:<15}{}: {:.2f} fps ({}/{})\n",
    KEY,
    "Frame Rate",
    RESET,
    static_cast<double>(video_stream.fps_num) / video_stream.fps_den,
    video_stream.fps_num,
    video_stream.fps_den
  );
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Bit Rate", RESET, format_bitrate(video_stream.bit_rate));
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Pixel Format", RESET, video_stream.pixel_format);

  fmt::print(
    "  {}{:<15}{}: Space: {} | Primaries: {} | TRC: {} | HDR: {}\n",
    KEY,
    "Color Details",
    RESET,
    video_stream.color_space.empty() ? "Unknown" : video_stream.color_space,
    video_stream.color_primaries.empty() ? "Unknown" : video_stream.color_primaries,
    video_stream.color_trc.empty() ? "Unknown" : video_stream.color_trc,
    video_stream.is_hdr ? "\033[32mYes\033[0m" : "No"
  );
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Field Order", RESET, video_stream.field_order);
  fmt::print(
    "  {}{:<15}{}: {}/{} (Start: {:.3f}s)\n",
    KEY,
    "Time Base",
    RESET,
    video_stream.time_base_num,
    video_stream.time_base_den,
    video_stream.start_time
  );
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Total Frames", RESET, video_stream.total_frames);

  print_tags(video_stream.tags, "  ");
  fmt::print("\n");
}

// -------------------------------------------------------------------------------------------------
void ProbeCommand::print_audio_stream_info(const AudioStream& audio_stream)
{
  fmt::print("{}{}--- Audio Stream #{} ---{}\n", STREAM, BOLD, audio_stream.index, RESET);
  fmt::print(
    "  {}{:<15}{}: {} (Profile: {})\n",
    KEY,
    "Codec",
    RESET,
    audio_stream.codec_name,
    audio_stream.profile
  );
  fmt::print("  {}{:<15}{}: {} Hz\n", KEY, "Sample Rate", RESET, audio_stream.sample_rate);
  fmt::print(
    "  {}{:<15}{}: {} ({})\n",
    KEY,
    "Channels",
    RESET,
    audio_stream.channels,
    audio_stream.channel_layout
  );
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Format", RESET, audio_stream.sample_format);
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Bit Rate", RESET, format_bitrate(audio_stream.bit_rate));
  fmt::print(
    "  {}{:<15}{}: {}/{} (Start: {:.3f}s)\n",
    KEY,
    "Time Base",
    RESET,
    audio_stream.time_base_num,
    audio_stream.time_base_den,
    audio_stream.start_time
  );
  fmt::print("  {}{:<15}{}: {}\n", KEY, "Total Frames", RESET, audio_stream.total_frames);

  print_tags(audio_stream.tags, "  ");
  fmt::print("\n");
}
