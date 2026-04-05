#include "cli/Parser.h"

int main(int argc, char** argv) {
    CLParser parser;
    std::unique_ptr<ICommand> command = parser.parse_arguments(argc, argv);
    if (command) {
        command->execute();
    }
}