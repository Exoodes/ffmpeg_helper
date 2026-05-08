#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "ICommand.h"

struct BitrateCommandPayload
{
  std::string input_path;
  std::string output_path;
  uint32_t interval_ms;
  std::vector<uint32_t> streams;
  bool aggregate = false;
};

class BitrateCommand : public ICommand
{
public:
  BitrateCommand(BitrateCommandPayload payload);
  void execute() override;

private:
  BitrateCommandPayload _payload;
};