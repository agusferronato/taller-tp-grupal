#include "PlayerAppearedEventParser.h"
#include "exceptions/protocol_error.h"

void PlayerAppearedEventParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& eventDTO = dynamic_cast<PlayerAppearedEventDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PLAYER_APPEARED), bytes);
    utils.appendToSend(eventDTO.getPlayerId(), bytes);
    utils.appendToSend(eventDTO.getX(), bytes);
    utils.appendToSend(eventDTO.getY(), bytes);
    bytes.push_back(static_cast<uint8_t>(eventDTO.getDirection()));
}

std::unique_ptr<CommandDTO> PlayerAppearedEventParser::getDTO(Protocol& protocol) {
    uint32_t player_id = protocol.getUint32();
    int16_t x = protocol.getInt16();
    int16_t y = protocol.getInt16();
    uint8_t raw_dir = protocol.getUint8();
    switch (static_cast<Direction>(raw_dir)) {
    case Direction::Up:
    case Direction::Down:
    case Direction::Left:
    case Direction::Right:
        return std::make_unique<PlayerAppearedEventDTO>(player_id, x, y, static_cast<Direction>(raw_dir));
    default:
        throw ProtocolError("Invalid direction in PlayerAppearedEvent");
    }
}
