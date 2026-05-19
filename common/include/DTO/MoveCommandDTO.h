#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "CommandDTO.h"
#include "Direction.h"
#include "ProtocolCodes.h"
#include <memory>

class MoveCommandDTO : public CommandDTO {
private:
  uint32_t player_id;
  Direction direction;

public:
  MoveCommandDTO(uint32_t player_id, Direction direction);

  uint32_t getPlayerId() const;
  Direction getDirection() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
