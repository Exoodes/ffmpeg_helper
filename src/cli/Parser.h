#include <memory>

#include <CLI/CLI.hpp>

#include "commands/CastCommand.h"
#include "commands/ICommand.h"
#include "commands/ProbeCommand.h"

class CLParser
{
public:
  std::unique_ptr<ICommand> parse_arguments(int argc, char** argv);

private:
  CLI::App* add_probe_subcommand(CLI::App& app, ProbeCommandPayload& payload);
  CLI::App* add_cast_subcommand(CLI::App& app, CastCommandPayload& payload);
};