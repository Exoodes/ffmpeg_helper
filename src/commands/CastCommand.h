#pragma once

#include "ICommand.h"

#include <string>

class CastCommand : public ICommand {
public:
    CastCommand(const std::string& cast_input, const std::string& target_url);
    void execute() override;

private:
    std::string _cast_input;
    std::string _target_url;
};