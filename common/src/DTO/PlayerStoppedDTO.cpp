#include "PlayerStoppedDTO.h"

PlayerStoppedDTO::PlayerStoppedDTO(uint32_t playerID) : playerID(playerID) {}

uint8_t PlayerStoppedDTO::getCode() {

  return static_cast<uint8_t>(ServerOpcode::PlayerStopped);
}

std::unique_ptr<CommandDTO> PlayerStoppedDTO::clone() {

  return std::make_unique<PlayerStoppedDTO>(*this);
}

uint32_t PlayerStoppedDTO::getPlayerID() { return playerID; }
