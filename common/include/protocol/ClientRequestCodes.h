#ifndef CLIENT_REQUEST_CODES_H
#define CLIENT_REQUEST_CODES_H

#include "DTO/Commands/ClientRequestDTO.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <type_traits>
#include <variant>

inline CommandOpCode getCode(const ClientRequestDTO &command) {
  return std::visit(
      [](const auto &concreteCommand) { return concreteCommand.getCode(); },
      command);
}

#endif
