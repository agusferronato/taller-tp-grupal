#ifndef NPC_DEFEATED_EVENT_DTO_H
#define NPC_DEFEATED_EVENT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <cstdint>
#include <memory>

class NpcDefeatedEventDTO : public CommandDTO {
private:
  uint8_t npc_id;

public:
  explicit NpcDefeatedEventDTO(uint8_t npc_id);

  uint8_t getNpcId() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
