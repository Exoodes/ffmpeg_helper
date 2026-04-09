#pragma once

#include <string>

#include <media/MediaAnalyzer.h>

#include "ICommand.h"

class ProbeCommand : public ICommand
{
public:
  ProbeCommand(const std::string& probe_input, const std::string& filename);
  void execute() override;

private:
  void print_media_properties(const MediaProperties& properties);

  std::string _probe_input;
  std::string _filename;
};