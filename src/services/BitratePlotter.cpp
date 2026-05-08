#include "BitratePlotter.hpp"

#include <fmt/core.h>
#include <matplot/matplot.h>

void BitratePlotter::plot(
  const std::map<uint32_t, BitrateHistogram>& histograms,
  const std::string& output_path
)
{
  matplot::cla();
  matplot::hold(matplot::on);

  for(const auto& pair : histograms) {
    uint32_t stream_index = pair.first;
    const BitrateHistogram& histogram = pair.second;

    std::vector<double> x_time;
    std::vector<double> y_bitrate_mbps;

    double interval_sec = histogram.time_interval_ms / 1000.0;

    for(const auto& data_pair : histogram.data) {
      uint32_t interval_index = data_pair.first;
      uint32_t bytes = data_pair.second;

      x_time.push_back(interval_index * interval_sec);

      double mbps = (static_cast<double>(bytes) * 8.0) / (interval_sec * 1000000.0);
      y_bitrate_mbps.push_back(mbps);
    }

    auto line = matplot::plot(x_time, y_bitrate_mbps);
    line->line_width(2);
    line->display_name(fmt::format("Stream {}", stream_index));
  }

  matplot::xlabel("Time (seconds)");
  matplot::ylabel("Bitrate (Mbps)");
  matplot::legend();
  matplot::save(output_path);

  fmt::print("Plot successfully saved to: {}\n", output_path);
}