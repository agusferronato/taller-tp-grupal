#ifndef CLIENT_MESSAGE_H
#define CLIENT_MESSAGE_H

#include <cstdint>

#include "DTO/Commands/ClientCommandDTO.h"

struct ClientMessage {
    ClientCommandDTO dto;
    uint32_t connectionId;
};

#endif
