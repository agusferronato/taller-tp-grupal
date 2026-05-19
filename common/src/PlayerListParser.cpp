#include "PlayerListParser.h"

void PlayerListParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& listDTO = dynamic_cast<PlayerListDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PlayerList), bytes);
    uint16_t count = static_cast<uint16_t>(listDTO.getPlayers().size());
    utils.appendToSend(count, bytes);
    for (const auto& player : listDTO.getPlayers()) {
        utils.appendToSend(player.player_id, bytes);
        utils.appendToSend(player.x, bytes);
        utils.appendToSend(player.y, bytes);
        utils.appendToSend(static_cast<uint8_t>(player.direction), bytes);
    }
}

std::unique_ptr<CommandDTO> PlayerListParser::getDTO(Protocol& protocol) {
    uint16_t count = protocol.getUint16();
    std::vector<PlayerInfoDTO> players;
    players.reserve(count);
    for (uint16_t i = 0; i < count; i++) {
        uint32_t pid = protocol.getUint32();
        int16_t x = protocol.getInt16();
        int16_t y = protocol.getInt16();
        Direction dir = static_cast<Direction>(protocol.getUint8());
        players.push_back({pid, x, y, dir});
    }
    return std::make_unique<PlayerListDTO>(std::move(players));
}
