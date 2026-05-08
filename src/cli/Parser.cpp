#include "Parser.h"

#include "commands/CastCommand.h"
#include "commands/ProbeCommand.h"

std::unique_ptr<ICommand> CLParser::parse_arguments(int argc, char** argv)
{
  CLI::App app{"FFmpeg Wrapper CLI"};
  app.require_subcommand(1);

  ProbeCommandPayload probe_payload;
  CLI::App* probe_subcommand = add_probe_subcommand(app, probe_payload);

  CastCommandPayload cast_payload;
  CLI::App* cast_subcommand = add_cast_subcommand(app, cast_payload);

  BitrateCommandPayload bitrate_payload;
  CLI::App* bitrate_subcommand = add_bitrate_subcommand(app, bitrate_payload);

  try {
    app.parse(argc, argv);
  } catch(const CLI::ParseError& e) {
    std::exit(app.exit(e));
  }

  if(*probe_subcommand) {
    return std::make_unique<ProbeCommand>(probe_payload);
  }

  if(*cast_subcommand) {
    return std::make_unique<CastCommand>(cast_payload);
  }

  if(*bitrate_subcommand) {
    return std::make_unique<BitrateCommand>(bitrate_payload);
  }

  return nullptr;
}

// -------------------------------------------------------------------------------------------------
CLI::App* CLParser::add_probe_subcommand(CLI::App& app, ProbeCommandPayload& payload)
{
  CLI::App* probe_subcommand = app.add_subcommand("probe", "Probe media files");

  probe_subcommand->add_option("input-path", payload.input_path, "Path to the input file")
    ->required();
  probe_subcommand->add_option("--config-file", payload.config_file, "Path to the config file");

  return probe_subcommand;
}

// -------------------------------------------------------------------------------------------------
CLI::App* CLParser::add_cast_subcommand(CLI::App& app, CastCommandPayload& payload)
{
  CLI::App* cast_subcommand = app.add_subcommand("cast", "Cast media files");

  cast_subcommand->add_option("input-path", payload.input_path, "Path to the input file")
    ->required();
  cast_subcommand->add_option("--target", payload.target_url, "Destination URL")->required();

  return cast_subcommand;
}

// -------------------------------------------------------------------------------------------------
CLI::App* CLParser::add_bitrate_subcommand(CLI::App& app, BitrateCommandPayload& payload)
{
  CLI::App* bitrate_subcommand = app.add_subcommand("bitrate", "Calculate bitrate of media files");

  bitrate_subcommand->add_option("input-path", payload.input_path, "Path to the input file")
    ->required();

  bitrate_subcommand->add_option("output-path", payload.output_path, "Path to the output file")
    ->default_val("bitrate_plot.png");

  bitrate_subcommand
    ->add_option("--interval", payload.interval_ms, "Time interval in ms for bitrate calculation")
    ->default_val(500);

  return bitrate_subcommand;
}