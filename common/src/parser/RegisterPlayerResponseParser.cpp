#include "RegisterPlayerResponseParser.h"

void RegisterPlayerResponseParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& respDTO = dynamic_cast<RegisterPlayerResponseDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(ServerOpcode::RegisterResponse), bytes);
    utils.appendToSend(respDTO.getPlayerId(), bytes);
    bytes.push_back(respDTO.getStatus());
}

std::unique_ptr<CommandDTO> RegisterPlayerResponseParser::getDTO(Protocol& protocol) {
    uint32_t player_id = protocol.getUint32();
    uint8_t status = protocol.getUint8();
    return std::make_unique<RegisterPlayerResponseDTO>(player_id, status);
}
