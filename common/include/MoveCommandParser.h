#ifndef MOVE_COMMAND_PARSER_H
#define MOVE_COMMAND_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "MoveCommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"
#include "exceptions/protocol_error.h"
#include <vector>
#include <memory>

class MoveCommandParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        auto& moveDTO = dynamic_cast<MoveCommandDTO&>(dto);
        utils.appendToSend(static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND), bytes);
        bytes.push_back(static_cast<uint8_t>(moveDTO.getDirection()));
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        uint8_t raw = protocol.getUint8();
        switch (static_cast<Direction>(raw)) {
        case Direction::UP:
        case Direction::DOWN:
        case Direction::LEFT:
        case Direction::RIGHT:
            return std::make_unique<MoveCommandDTO>(static_cast<Direction>(raw));
        default:
            throw ProtocolError("Invalid direction received");
        }
    }
};

#endif
