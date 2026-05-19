#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "CommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"
#include <cstdint>
#include <memory>
#include <vector>

struct PlayerInfoDTO {
  uint32_t player_id;
  int16_t x;
  int16_t y;
  Direction direction;
};

class PlayerListDTO : public CommandDTO {
private:
  std::vector<PlayerInfoDTO> players;

public:
  explicit PlayerListDTO(std::vector<PlayerInfoDTO> players);

  const std::vector<PlayerInfoDTO> &getPlayers() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
