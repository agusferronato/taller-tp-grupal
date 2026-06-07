#ifndef ATTACK_RECEIVED_EVENT_DTO_H
#define ATTACK_RECEIVED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct AttackReceivedEventDTO {
    static constexpr EventOpcode opCode = EventOpcode::AttackReceivedEvent;
    EventOpcode getCode() const { return opCode; }
    uint32_t damage;
};

#endif
