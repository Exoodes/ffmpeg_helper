#include "CastCommand.h"

#include <fmt/core.h>

CastCommand::CastCommand(const CastCommandPayload& payload)
  : _payload(payload)
{
}

void CastCommand::execute()
{
  fmt::print(
    "Executing cast with input path: {} and target URL: {}\n",
    _payload.input_path,
    _payload.target_url
  );
}