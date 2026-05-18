#include "PlayerListDTO.h"

PlayerListDTO::PlayerListDTO(std::vector<PlayerId> player_ids)
    : player_ids(std::move(player_ids)) {}

const std::vector<PlayerId>& PlayerListDTO::getPlayerIds() const { return player_ids; }

uint8_t PlayerListDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::PLAYER_LIST);
}

std::unique_ptr<CommandDTO> PlayerListDTO::clone() {
    return std::make_unique<PlayerListDTO>(*this);
}
