#include "RegisterPlayerResponseDTO.h"

RegisterPlayerResponseDTO::RegisterPlayerResponseDTO(PlayerId player_id, uint8_t status)
    : player_id(player_id), status(status) {}

PlayerId RegisterPlayerResponseDTO::getPlayerId() const { return player_id; }

uint8_t RegisterPlayerResponseDTO::getStatus() const { return status; }

uint8_t RegisterPlayerResponseDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::REGISTER_RESPONSE);
}

std::unique_ptr<CommandDTO> RegisterPlayerResponseDTO::clone() {
    return std::make_unique<RegisterPlayerResponseDTO>(*this);
}
