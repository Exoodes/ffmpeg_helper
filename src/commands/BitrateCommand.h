#pragma once

#include <string>

#include "ICommand.h"

struct BitrateCommandPayload
{
  std::string input_path;
  std::string output_path;
};

class BitrateCommand : public ICommand
{
public:
  BitrateCommand(const BitrateCommandPayload& payload);
  void execute() override;

private:
  BitrateCommandPayload _payload;
};