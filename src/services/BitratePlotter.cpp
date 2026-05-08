#include "BitratePlotter.hpp"

#include <algorithm>
#include <fmt/core.h>
#include <matplot/matplot.h>
#include <numeric>

void BitratePlotter::plot(
  const std::map<std::string, BitrateHistogram>& histograms,
  const std::string& output_path
)
{
  matplot::gcf()->size(1920, 1080);

  matplot::cla();
  matplot::hold(matplot::on);

  std::vector<std::string> legend_labels;
  double global_max_mbps = 0.0;

  for(const auto& pair : histograms) {
    std::string label = pair.first;
    const BitrateHistogram& histogram = pair.second;

    std::vector<double> x_time;
    std::vector<double> y_bitrate_mbps;

    double interval_sec = histogram.time_interval_ms / 1000.0;
    double max_x = 0.0;

    for(const auto& data_pair : histogram.data) {
      uint32_t interval_index = data_pair.first;
      uint32_t bytes = data_pair.second;

      double current_x = interval_index * interval_sec;
      x_time.push_back(current_x);
      max_x = std::max(max_x, current_x);

      double mbps = (static_cast<double>(bytes) * 8.0) / (interval_sec * 1000000.0);
      y_bitrate_mbps.push_back(mbps);
    }

    if(y_bitrate_mbps.empty()) {
      continue;
    }

    double sum = std::accumulate(y_bitrate_mbps.begin(), y_bitrate_mbps.end(), 0.0);
    double mean_mbps = sum / y_bitrate_mbps.size();
    double max_mbps = *std::max_element(y_bitrate_mbps.begin(), y_bitrate_mbps.end());

    global_max_mbps = std::max(global_max_mbps, max_mbps);

    auto line = matplot::plot(x_time, y_bitrate_mbps);
    line->line_width(2);
    legend_labels.push_back(label);

    auto mean_line =
      matplot::plot(std::vector<double>{0, max_x}, std::vector<double>{mean_mbps, mean_mbps});
    mean_line->line_style("--");
    mean_line->line_width(1.5);
    legend_labels.push_back(fmt::format("{} (Mean: {:.2f} Mbps)", label, mean_mbps));

    auto max_line =
      matplot::plot(std::vector<double>{0, max_x}, std::vector<double>{max_mbps, max_mbps});
    max_line->line_style(":");
    max_line->line_width(1.5);
    legend_labels.push_back(fmt::format("{} (Max: {:.2f} Mbps)", label, max_mbps));
  }

  if(global_max_mbps > 0) {
    matplot::ylim({0, global_max_mbps * 1.35});
  }

  auto l = matplot::legend(legend_labels);
  l->box(false);

  matplot::xlabel("Time (seconds)");
  matplot::ylabel("Bitrate (Mbps)");
  matplot::save(output_path);

  fmt::print("Plot successfully saved to: {}\n", output_path);
}