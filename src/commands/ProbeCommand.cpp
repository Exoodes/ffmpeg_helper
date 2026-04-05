#include "ProbeCommand.h"
    
#include <fmt/core.h>


ProbeCommand::ProbeCommand(
    const std::string& probe_input,
    const std::string& filename
)
    : _probe_input(probe_input)
    , _filename(filename) 
{}

void ProbeCommand::execute()
{
    fmt::print("Executing probe with input path: {} and config file: {}\n", _probe_input, _filename);
}