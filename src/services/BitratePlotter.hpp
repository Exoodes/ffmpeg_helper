#pragma once

#include <map>
#include <string>

#include "media/MediaAnalyzer.h"

class BitratePlotter
{
public:
  static void plot(
    const std::map<std::string, BitrateHistogram>& histograms,
    const std::string& output_path
  );
};