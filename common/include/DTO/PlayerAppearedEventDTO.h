#ifndef PLAYER_APPEARED_EVENT_DTO_H
#define PLAYER_APPEARED_EVENT_DTO_H

#include "CommandDTO.h"
#include "Direction.h"
#include "ProtocolCodes.h"
#include <cstdint>
#include <memory>

class PlayerAppearedEventDTO : public CommandDTO {
private:
  uint32_t player_id;
  int16_t x;
  int16_t y;
  Direction direction;

public:
  PlayerAppearedEventDTO(uint32_t player_id, int16_t x, int16_t y,
                         Direction direction);

  uint32_t getPlayerId() const;
  int16_t getX() const;
  int16_t getY() const;
  Direction getDirection() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
