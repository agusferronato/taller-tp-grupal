#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>

#include "Command.h"
#include "CommandDTO.h"

class CommandFactory {
public:
    std::unique_ptr<Command> create(std::unique_ptr<CommandDTO> dto);
};

#endif // COMMAND_FACTORY_H
