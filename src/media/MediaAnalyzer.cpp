#include "MediaAnalyzer.h"

#include <fmt/core.h>

#include "FFmpegHeaders.h"
#include "Utility.h"

// -------------------------------------------------------------------------------------------------
BitrateScanner::BitrateScanner(uint32_t time_interval_ms, AVFormatContextPtr ctx)
  : _time_interval_ms(time_interval_ms)
  , _ctx(std::move(ctx))
{
}

// -------------------------------------------------------------------------------------------------
std::map<uint32_t, BitrateHistogram> BitrateScanner::scan()
{
  std::map<uint32_t, BitrateHistogram> histograms;

  AVPacket* packet = av_packet_alloc();

  while(av_read_frame(_ctx.get(), packet) >= 0) {
    int stream_index = packet->stream_index;
    int64_t pts = packet->pts;
    AVRational time_base = _ctx->streams[stream_index]->time_base;

    double time_seconds = pts * av_q2d(time_base);
    uint32_t time_ms = static_cast<uint32_t>(time_seconds * 1000);
    uint32_t interval_index = time_ms / _time_interval_ms;

    histograms[stream_index].time_interval_ms = _time_interval_ms;
    histograms[stream_index].data[interval_index] += packet->size;

    av_packet_unref(packet);
  }

  av_packet_free(&packet);

  return histograms;
}

// -------------------------------------------------------------------------------------------------
MediaAnalyzer::MediaAnalyzer(const std::string& file_path)
  : _file_path(file_path)
{
}

// -------------------------------------------------------------------------------------------------
std::map<std::string, std::string> MediaAnalyzer::extract_metadata(AVDictionary* dict)
{
  std::map<std::string, std::string> tags;
  AVDictionaryEntry* tag = nullptr;
  while((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
    tags[tag->key] = tag->value;
  }
  return tags;
}

// -------------------------------------------------------------------------------------------------
MediaProperties MediaAnalyzer::analyze()
{
  AVFormatContextPtr ctx = open_input(_file_path);
  if(!ctx) {
    return {};
  }

  int result = avformat_find_stream_info(ctx.get(), nullptr);
  if(logAVERROR(result)) {
    return {};
  }

  MediaProperties properties;
  properties.container_name = ctx->iformat->name;
  properties.container_long_name = ctx->iformat->long_name;

  properties.start_time_seconds = ctx->start_time == AV_NOPTS_VALUE
                                    ? 0.0
                                    : ctx->start_time / static_cast<double>(AV_TIME_BASE);
  properties.duration_seconds = ctx->duration / static_cast<double>(AV_TIME_BASE);
  properties.overall_bit_rate = ctx->bit_rate;
  properties.file_size_bytes = avio_size(ctx->pb);

  properties.tags = extract_metadata(ctx->metadata);

  for(unsigned int i = 0; i < ctx->nb_chapters; ++i) {
    auto* chap = ctx->chapters[i];
    Chapter chapter;
    chapter.id = chap->id;
    double tb = av_q2d(chap->time_base);
    chapter.start_time = chap->start == AV_NOPTS_VALUE ? 0.0 : chap->start * tb;
    chapter.end_time = chap->end == AV_NOPTS_VALUE ? 0.0 : chap->end * tb;
    chapter.tags = extract_metadata(chap->metadata);
    properties.chapters.push_back(chapter);
  }

  for(unsigned int i = 0; i < ctx->nb_streams; ++i) {
    auto* stream = ctx->streams[i];
    if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      properties.video_streams.push_back(extract_video_stream_info(stream));
    } else if(stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      properties.audio_streams.push_back(extract_audio_stream_info(stream));
    }
  }

  return properties;
}

// -------------------------------------------------------------------------------------------------
std::map<uint32_t, BitrateHistogram> MediaAnalyzer::analyze_bitrate(uint32_t time_interval_ms)
{
  AVFormatContextPtr ctx = open_input(_file_path);
  if(!ctx) {
    return {};
  }

  BitrateScanner scanner(time_interval_ms, std::move(ctx));
  std::map<uint32_t, BitrateHistogram> histograms = scanner.scan();
  return histograms;
}

// -------------------------------------------------------------------------------------------------
AVFormatContextPtr MediaAnalyzer::open_input(const std::string& file_path)
{
  AVFormatContext* ctx_raw = nullptr;
  int result = avformat_open_input(&ctx_raw, file_path.c_str(), nullptr, nullptr);
  if(logAVERROR(result)) {
    return nullptr;
  }

  return AVFormatContextPtr(ctx_raw);
}

// -------------------------------------------------------------------------------------------------
VideoStream MediaAnalyzer::extract_video_stream_info(AVStream* stream)
{
  VideoStream video_stream;

  video_stream.index = stream->index;
  video_stream.codec_name = getStringFromCharArray(avcodec_get_name(stream->codecpar->codec_id));
  video_stream.profile = getStringFromCharArray(
    avcodec_profile_name(stream->codecpar->codec_id, stream->codecpar->profile)
  );

  video_stream.width = stream->codecpar->width;
  video_stream.height = stream->codecpar->height;
  video_stream.fps_num = stream->avg_frame_rate.num;
  video_stream.fps_den = stream->avg_frame_rate.den;
  video_stream.bit_rate = stream->codecpar->bit_rate;

  video_stream.pixel_format = getStringFromCharArray(
    av_get_pix_fmt_name(static_cast<AVPixelFormat>(stream->codecpar->format))
  );

  video_stream.color_space = getStringFromCharArray(
    av_color_space_name(stream->codecpar->color_space)
  );
  video_stream.color_primaries = getStringFromCharArray(
    av_color_primaries_name(stream->codecpar->color_primaries)
  );
  video_stream.color_trc = getStringFromCharArray(
    av_color_transfer_name(stream->codecpar->color_trc)
  );

  video_stream.is_hdr =
    (stream->codecpar->color_trc == AVCOL_TRC_SMPTE2084 ||
     stream->codecpar->color_trc == AVCOL_TRC_ARIB_STD_B67);

  switch(stream->codecpar->field_order) {
  case AV_FIELD_PROGRESSIVE:
    video_stream.field_order = "Progressive";
    break;
  case AV_FIELD_TT:
    video_stream.field_order = "Top Field First";
    break;
  case AV_FIELD_BB:
    video_stream.field_order = "Bottom Field First";
    break;
  case AV_FIELD_TB:
    video_stream.field_order = "Top Bottom";
    break;
  case AV_FIELD_BT:
    video_stream.field_order = "Bottom Top";
    break;
  default:
    video_stream.field_order = "Unknown";
    break;
  }

  video_stream.time_base_num = stream->time_base.num;
  video_stream.time_base_den = stream->time_base.den;
  video_stream.start_time = stream->start_time == AV_NOPTS_VALUE
                              ? 0.0
                              : stream->start_time * av_q2d(stream->time_base);
  video_stream.total_frames = stream->nb_frames;

  video_stream.tags = extract_metadata(stream->metadata);

  return video_stream;
}

// -------------------------------------------------------------------------------------------------
AudioStream MediaAnalyzer::extract_audio_stream_info(AVStream* stream)
{
  AudioStream audio_stream;

  audio_stream.index = stream->index;
  audio_stream.codec_name = getStringFromCharArray(avcodec_get_name(stream->codecpar->codec_id));
  audio_stream.profile = getStringFromCharArray(
    avcodec_profile_name(stream->codecpar->codec_id, stream->codecpar->profile)
  );

  audio_stream.sample_rate = stream->codecpar->sample_rate;
  audio_stream.channels = stream->codecpar->ch_layout.nb_channels;
  char channel_layout_buf[128];
  av_channel_layout_describe(
    &stream->codecpar->ch_layout,
    channel_layout_buf,
    sizeof(channel_layout_buf)
  );
  audio_stream.channel_layout = channel_layout_buf;
  audio_stream.sample_format = getStringFromCharArray(
    av_get_sample_fmt_name(static_cast<AVSampleFormat>(stream->codecpar->format))
  );
  audio_stream.bit_rate = stream->codecpar->bit_rate;

  audio_stream.time_base_num = stream->time_base.num;
  audio_stream.time_base_den = stream->time_base.den;
  audio_stream.start_time = stream->start_time == AV_NOPTS_VALUE
                              ? 0.0
                              : stream->start_time * av_q2d(stream->time_base);
  audio_stream.total_frames = stream->nb_frames;

  audio_stream.tags = extract_metadata(stream->metadata);

  return audio_stream;
}