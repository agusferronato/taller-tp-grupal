#ifndef PLAYER_MOVED_EVENT_PARSER_H
#define PLAYER_MOVED_EVENT_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "PlayerMovedEventDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class PlayerMovedEventParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        auto& eventDTO = dynamic_cast<PlayerMovedEventDTO&>(dto);
        utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PLAYER_MOVED), bytes);
        utils.appendToSend(eventDTO.getPlayerId(), bytes);
        utils.appendToSend(eventDTO.getX(), bytes);
        utils.appendToSend(eventDTO.getY(), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        uint32_t player_id = protocol.getUint32();
        uint16_t x = protocol.getUint16();
        uint16_t y = protocol.getUint16();
        return std::make_unique<PlayerMovedEventDTO>(player_id, x, y);
    }
};

#endif
