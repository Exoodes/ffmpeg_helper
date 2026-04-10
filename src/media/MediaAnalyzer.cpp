#include "MediaAnalyzer.h"

#include "FFmpegHeaders.h"
#include "Utility.h"

// -------------------------------------------------------------------------------------------------
MediaAnalyzer::MediaAnalyzer(const std::string& file_path)
  : _file_path(file_path)
{
}

// -------------------------------------------------------------------------------------------------
MediaProperties MediaAnalyzer::analyze()
{
  int result;
  AVFormatContext* ctx_raw = nullptr;

  result = avformat_open_input(&ctx_raw, _file_path.c_str(), nullptr, nullptr);
  AVFormatContextPtr ctx(ctx_raw);

  if(logAVERROR(result)) {
    return {};
  }

  result = avformat_find_stream_info(ctx.get(), nullptr);
  if(logAVERROR(result)) {
    return {};
  }

  MediaProperties properties;
  properties.container_name = ctx->iformat->name;
  properties.container_long_name = ctx->iformat->long_name;
  properties.duration_seconds = ctx->duration / static_cast<double>(AV_TIME_BASE);
  properties.overall_bit_rate = ctx->bit_rate;
  properties.file_size_bytes = avio_size(ctx->pb);

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
VideoStream MediaAnalyzer::extract_video_stream_info(AVStream* stream)
{
  VideoStream video_stream;

  video_stream.index = stream->index;
  video_stream.codec_name = avcodec_get_name(stream->codecpar->codec_id);
  video_stream.profile =
    avcodec_profile_name(stream->codecpar->codec_id, stream->codecpar->profile);

  video_stream.width = stream->codecpar->width;
  video_stream.height = stream->codecpar->height;
  video_stream.fps_num = stream->avg_frame_rate.num;
  video_stream.fps_den = stream->avg_frame_rate.den;
  video_stream.bit_rate = stream->codecpar->bit_rate;

  video_stream.pixel_format = av_get_pix_fmt_name(
    static_cast<AVPixelFormat>(stream->codecpar->format)
  );

  return video_stream;
}

// -------------------------------------------------------------------------------------------------
AudioStream MediaAnalyzer::extract_audio_stream_info(AVStream* stream)
{
  AudioStream audio_stream;

  audio_stream.index = stream->index;
  audio_stream.codec_name = avcodec_get_name(stream->codecpar->codec_id);
  audio_stream.profile =
    avcodec_profile_name(stream->codecpar->codec_id, stream->codecpar->profile);

  audio_stream.sample_rate = stream->codecpar->sample_rate;
  audio_stream.channels = stream->codecpar->ch_layout.nb_channels;
  char channel_layout_buf[128];
  av_channel_layout_describe(
    &stream->codecpar->ch_layout,
    channel_layout_buf,
    sizeof(channel_layout_buf)
  );
  audio_stream.channel_layout = channel_layout_buf;
  audio_stream.sample_format = av_get_sample_fmt_name(
    static_cast<AVSampleFormat>(stream->codecpar->format)
  );
  audio_stream.bit_rate = stream->codecpar->bit_rate;

  return audio_stream;
}
