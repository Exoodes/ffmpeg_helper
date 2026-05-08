#pragma once

#include <string>

#include "ICommand.h"

struct CastCommandPayload
{
  std::string input_path;
  std::string target_url;
};

class CastCommand : public ICommand
{
public:
  CastCommand(const CastCommandPayload& payload);
  void execute() override;

private:
  CastCommandPayload _payload;
};