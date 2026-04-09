#pragma once

#include <memory>

#include "FFmpegHeaders.h"

struct AVFormatContextDeleter
{
  void operator()(AVFormatContext* ctx) const
  {
    if(ctx) {
      avformat_close_input(&ctx); // Maybe avformat_free_context should be used instead?
    }
  }
};

using AVFormatContextPtr = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>;