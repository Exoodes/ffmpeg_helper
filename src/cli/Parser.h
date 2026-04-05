#include "commands/ICommand.h"

#include <memory>

#include <CLI/CLI.hpp>

class CLParser {
public:
    std::unique_ptr<ICommand> parse_arguments(int argc, char** argv);
};