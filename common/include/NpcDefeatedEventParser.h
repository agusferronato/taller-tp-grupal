#ifndef NPC_DEFEATED_EVENT_PARSER_H
#define NPC_DEFEATED_EVENT_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "NpcDefeatedEventDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class NpcDefeatedEventParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        auto& eventDTO = dynamic_cast<NpcDefeatedEventDTO&>(dto);
        utils.appendToSend(static_cast<uint8_t>(ServerOpcode::NPC_DEFEATED), bytes);
        utils.appendToSend(eventDTO.getNpcId(), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        uint8_t npc_id = protocol.getUint8();
        return std::make_unique<NpcDefeatedEventDTO>(npc_id);
    }
};

#endif
