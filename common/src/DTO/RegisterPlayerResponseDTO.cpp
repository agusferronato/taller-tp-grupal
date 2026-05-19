#include "RegisterPlayerResponseDTO.h"

RegisterPlayerResponseDTO::RegisterPlayerResponseDTO(uint32_t player_id,
                                                     uint8_t status)
    : player_id(player_id), status(status) {}

uint32_t RegisterPlayerResponseDTO::getPlayerId() const { return player_id; }

uint8_t RegisterPlayerResponseDTO::getStatus() const { return status; }

uint8_t RegisterPlayerResponseDTO::getCode() {
  return static_cast<uint8_t>(ServerOpcode::RegisterResponse);
}

std::unique_ptr<CommandDTO> RegisterPlayerResponseDTO::clone() {
  return std::make_unique<RegisterPlayerResponseDTO>(*this);
}
