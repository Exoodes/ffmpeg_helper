#include "Parser.h"
#include "commands/ProbeCommand.h"
#include "commands/CastCommand.h"


std::unique_ptr<ICommand> CLParser::parse_arguments(int argc, char** argv)
{
    CLI::App app{"FFmpeg Wrapper CLI"};

    app.require_subcommand(1);
    CLI::App* probe_subcommand = app.add_subcommand("probe", "Probe media files");
    
    std::string probe_input;
    std::string filename;
    probe_subcommand->add_option("input-path", probe_input, "Path to the input file")->required();
    probe_subcommand->add_option("--config-file", filename, "Path to the config file");

    CLI::App* cast_subcommand = app.add_subcommand("cast", "Cast media files");
    
    std::string cast_input;
    std::string target_url;
    cast_subcommand->add_option("input-path", cast_input, "Path to the input file")->required();
    cast_subcommand->add_option("--target", target_url, "Destination URL")->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::exit(app.exit(e));
    }

    if (*probe_subcommand) {
        return std::make_unique<ProbeCommand>(probe_input, filename);
    }

    if (*cast_subcommand) {
        return std::make_unique<CastCommand>(cast_input, target_url);
    }

    return nullptr;
}