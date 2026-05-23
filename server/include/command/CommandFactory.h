#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>

/*
    CommandFactory
    Convierte un CommandDTO en un Command concreto, dependiendo del codigo que
   tenga el DTO.
*/

#include "Command.h"
#include "CommandDTO.h"

class CommandFactory {
public:
  std::unique_ptr<Command> create(std::unique_ptr<CommandDTO> dto);
};

#endif // COMMAND_FACTORY_H
