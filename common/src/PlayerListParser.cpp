#include "PlayerListParser.h"

void PlayerListParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& listDTO = dynamic_cast<PlayerListDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PLAYER_LIST), bytes);
    uint16_t count = static_cast<uint16_t>(listDTO.getPlayerIds().size());
    utils.appendToSend(count, bytes);
    for (auto pid : listDTO.getPlayerIds()) {
        utils.appendToSend(pid, bytes);
    }
}

std::unique_ptr<CommandDTO> PlayerListParser::getDTO(Protocol& protocol) {
    uint16_t count = protocol.getUint16();
    std::vector<PlayerId> ids;
    ids.reserve(count);
    for (uint16_t i = 0; i < count; i++) {
        ids.push_back(protocol.getUint32());
    }
    return std::make_unique<PlayerListDTO>(std::move(ids));
}
