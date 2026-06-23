#include "protocol/ClientCommandCodes.h"

ClientCommandOpCode getCode(const ClientCommandDTO &command) {
  return std::visit(
      [](const auto &concreteCommand) { return concreteCommand.getCode(); },
      command);
}
