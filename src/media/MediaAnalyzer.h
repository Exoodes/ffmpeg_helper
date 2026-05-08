#pragma once

#include <map>
#include <string>
#include <vector>

#include "SmartPointers.h"

// -------------------------------------------------------------------------------------------------
struct Chapter
{
  int id;
  double start_time;
  double end_time;
  std::map<std::string, std::string> tags;
};

// -------------------------------------------------------------------------------------------------
struct VideoStream
{
  // Basic track info
  int index;              // The stream index (e.g., 0)
  std::string codec_name; // e.g., "h264", "hevc", "av1"
  std::string profile;    // e.g., "High", "Main 10", "Baseline"

  // Video dimensions and timings
  int width;        // e.g., 1920, 3840
  int height;       // e.g., 1080, 2160
  int fps_num;      // e.g., 30000
  int fps_den;      // e.g., 1001 (for 29.97 fps
  int64_t bit_rate; // e.g., 5000000 (5 Mbps)

  // // Advanced Picture, Color, and HDR properties
  std::string pixel_format;    // e.g., "yuv420p" (8-bit), "yuv420p10le" (10-bit)
  std::string field_order;     // e.g., "Progressive", "Top Field First" (Interlaced)
  std::string color_space;     // e.g., "bt709" (SDR), "bt2020nc" (HDR)
  std::string color_primaries; // e.g., "bt709", "bt2020"
  std::string color_trc;       // Transfer characteristics, e.g., "smpte2084" (PQ for HDR)
  bool is_hdr;                 // A helper boolean you can set if color_trc indicates HDR

  // Technical stream properties
  int time_base_num;
  int time_base_den;
  double start_time;
  int64_t total_frames;

  // Track Metadata Tags
  std::map<std::string, std::string> tags;
};

// -------------------------------------------------------------------------------------------------
struct AudioStream
{
  // Basic track info
  int index;
  std::string codec_name; // e.g., "aac", "flac", "ac3"
  std::string profile;    // e.g., "LC" for AAC-LC

  // Audio specifics
  int sample_rate;            // e.g., 48000
  int channels;               // e.g., 2, 6
  std::string channel_layout; // e.g., "stereo", "5.1(side)"
  std::string sample_format;  // e.g., "s16p" (16-bit), "fltp" (32-bit float)
  int64_t bit_rate;           // e.g., 320000 (320 kbps)

  // Technical stream properties
  int time_base_num;
  int time_base_den;
  double start_time;
  int64_t total_frames;

  // Track Metadata Tags
  std::map<std::string, std::string> tags;
};

// -------------------------------------------------------------------------------------------------
struct MediaProperties
{
  // Format details
  std::string container_name;      // e.g., "matroska,webm"
  std::string container_long_name; // e.g., "Matroska / WebM"

  // Timings and sizes
  double start_time_seconds;
  double duration_seconds;  // Converted from ctx->duration
  int64_t overall_bit_rate; // ctx->bit_rate
  int64_t file_size_bytes;  // avio_size(ctx->pb)

  // Global Metadata Tags
  std::map<std::string, std::string> tags;

  // The media tracks and chapters
  std::vector<VideoStream> video_streams;
  std::vector<AudioStream> audio_streams;
  std::vector<Chapter> chapters;
};

// -------------------------------------------------------------------------------------------------
class MediaAnalyzer
{
public:
  MediaAnalyzer(const std::string& file_path);
  MediaProperties analyze();

private:
  VideoStream extract_video_stream_info(AVStream* stream);
  AudioStream extract_audio_stream_info(AVStream* stream);
  std::map<std::string, std::string> extract_metadata(AVDictionary* dict);

  std::string _file_path;
};