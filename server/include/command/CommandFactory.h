#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>

/*
    CommandFactory
    Convierte un CommandDTO en un Command concreto, dependiendo del codigo que
   tenga el DTO.
*/

#include "Command.h"
#include "DTO/Commands/ClientCommandDTO.h"

#include <memory>

class CommandFactory {
public:
  std::unique_ptr<Command> create(const ClientCommandDTO &dto);
};

#endif // COMMAND_FACTORY_H
