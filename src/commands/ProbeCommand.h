#pragma once

#include "ICommand.h"

#include <string>

class ProbeCommand : public ICommand {
public:
    ProbeCommand(const std::string& probe_input, const std::string& filename);
    void execute() override;

private:
    std::string _probe_input;
    std::string _filename;
};