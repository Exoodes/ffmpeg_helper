#include "cli/Parser.h"

int main(int argc, char** argv)
{
  av_log_set_level(AV_LOG_ERROR);

  CLParser parser;
  std::unique_ptr<ICommand> command = parser.parse_arguments(argc, argv);
  if(command) {
    command->execute();
  }

  return 0;
}