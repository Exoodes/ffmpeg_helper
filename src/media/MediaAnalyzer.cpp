#include "MediaAnalyzer.h"

#include "FFmpegHeaders.h"
#include "Utility.h"

MediaAnalyzer::MediaAnalyzer(const std::string& file_path)
  : _file_path(file_path)
{
}

MediaProperties MediaAnalyzer::analyze()
{
  int result;
  AVFormatContext* ctx_raw = avformat_alloc_context();
  AVFormatContextPtr ctx(ctx_raw);

  result = avformat_open_input(&ctx_raw, _file_path.c_str(), nullptr, nullptr);
  if(logAVERROR(result)) {
    return {};
  }

  result = avformat_find_stream_info(ctx.get(), nullptr);
  if(logAVERROR(result)) {
    return {};
  }

  MediaProperties properties;
  properties.container_name = ctx->iformat->name;

  for(unsigned int i = 0; i < ctx->nb_streams; ++i) {
    auto* stream = ctx->streams[i];
    if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      VideoStream video_stream;
      video_stream.width = stream->codecpar->width;
      video_stream.height = stream->codecpar->height;
      properties.video_streams.push_back(video_stream);
    } else if(stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      AudioStream audio_stream;
      audio_stream.sample_rate = stream->codecpar->sample_rate;
      audio_stream.channels = stream->codecpar->ch_layout.nb_channels;
      properties.audio_streams.push_back(audio_stream);
    }
  }

  return properties;
}