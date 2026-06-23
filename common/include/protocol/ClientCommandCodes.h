#ifndef CLIENT_REQUEST_CODES_H
#define CLIENT_REQUEST_CODES_H

#include "DTO/Commands/ClientCommandDTO.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <type_traits>
#include <variant>

ClientCommandOpCode getCode(const ClientCommandDTO &command);

#endif
