#include "PlayerListDTO.h"

PlayerListDTO::PlayerListDTO(std::vector<PlayerInfoDTO> players)
    : players(std::move(players)) {}

const std::vector<PlayerInfoDTO> &PlayerListDTO::getPlayers() const {
  return players;
}

uint8_t PlayerListDTO::getCode() {
  return static_cast<uint8_t>(ServerOpcode::PlayerList);
}

std::unique_ptr<CommandDTO> PlayerListDTO::clone() {
  return std::make_unique<PlayerListDTO>(*this);
}
