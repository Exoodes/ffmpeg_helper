#include "BitrateCommand.h"

#include <algorithm>
#include <fmt/core.h>

#include <media/MediaAnalyzer.h>

#include "services/BitratePlotter.hpp"

BitrateCommand::BitrateCommand(BitrateCommandPayload payload)
  : _payload(std::move(payload))
{
}

// -------------------------------------------------------------------------------------------------
void BitrateCommand::execute()
{
  fmt::print("Analyzing bitrate for: {}\n", _payload.input_path);

  MediaAnalyzer analyzer(_payload.input_path);
  std::map<uint32_t, BitrateHistogram> raw_histograms = analyzer.analyze_bitrate(
    _payload.interval_ms
  );

  if(raw_histograms.empty()) {
    fmt::print("Error: No data extracted or file could not be opened.\n");
    return;
  }

  std::map<std::string, BitrateHistogram> processed_histograms;

  if(_payload.aggregate) {
    BitrateHistogram aggregated_hist;
    aggregated_hist.time_interval_ms = _payload.interval_ms;

    for(const auto& [stream_idx, hist] : raw_histograms) {
      if(
        !_payload.streams.empty() &&
        std::find(_payload.streams.begin(), _payload.streams.end(), stream_idx) ==
          _payload.streams.end()
      )
      {
        continue;
      }

      for(const auto& [interval, bytes] : hist.data) {
        aggregated_hist.data[interval] += bytes;
      }
    }

    if(!aggregated_hist.data.empty()) {
      processed_histograms["Total Aggregated Bitrate"] = aggregated_hist;
    }

  } else {
    for(const auto& [stream_idx, hist] : raw_histograms) {
      if(
        !_payload.streams.empty() &&
        std::find(_payload.streams.begin(), _payload.streams.end(), stream_idx) ==
          _payload.streams.end()
      )
      {
        continue;
      }

      processed_histograms[fmt::format("Stream {}", stream_idx)] = hist;
    }
  }

  if(processed_histograms.empty()) {
    fmt::print("Error: None of the requested streams were found in the file.\n");
    return;
  }

  BitratePlotter::plot(processed_histograms, _payload.output_path);
}