#include "CastCommand.h"

#include <fmt/core.h>


CastCommand::CastCommand(
    const std::string& cast_input,
    const std::string& target_url
)
    : _cast_input(cast_input)
    , _target_url(target_url) 
{}

void CastCommand::execute()
{
    fmt::print("Executing cast with input path: {} and target URL: {}\n", _cast_input, _target_url);
}