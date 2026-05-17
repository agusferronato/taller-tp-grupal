#ifndef NPC_DEFEATED_EVENT_DTO_H
#define NPC_DEFEATED_EVENT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <cstdint>

class NpcDefeatedEventDTO : public CommandDTO {
private:
    uint8_t npc_id;

public:
    explicit NpcDefeatedEventDTO(uint8_t npc_id) : npc_id(npc_id) {}

    uint8_t getNpcId() const { return npc_id; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(ServerOpcode::NPC_DEFEATED);
    }
};

#endif
