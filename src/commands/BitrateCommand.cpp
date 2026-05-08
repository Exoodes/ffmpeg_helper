#include "BitrateCommand.h"

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

  std::map<uint32_t, BitrateHistogram> histograms = analyzer.analyze_bitrate(_payload.interval_ms);

  if(histograms.empty()) {
    fmt::print("Error: No data extracted or file could not be opened.\n");
    return;
  }

  BitratePlotter::plot(histograms, _payload.output_path);
}