#include "MoveCommandParser.h"

void MoveCommandParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& moveDTO = dynamic_cast<MoveCommandDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND), bytes);
    bytes.push_back(static_cast<uint8_t>(moveDTO.getDirection()));
}

std::unique_ptr<CommandDTO> MoveCommandParser::getDTO(Protocol& protocol) {
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
