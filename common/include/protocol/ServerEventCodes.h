#ifndef SERVER_EVENT_CODES_H
#define SERVER_EVENT_CODES_H

#include "DTO/Events/EventDTO.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <type_traits>
#include <variant>

EventOpcode getCode(const ServerEventDTO &event);

#endif
