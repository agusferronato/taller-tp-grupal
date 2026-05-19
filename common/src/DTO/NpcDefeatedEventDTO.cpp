#include "NpcDefeatedEventDTO.h"

NpcDefeatedEventDTO::NpcDefeatedEventDTO(uint8_t npc_id) : npc_id(npc_id) {}

uint8_t NpcDefeatedEventDTO::getNpcId() const { return npc_id; }

uint8_t NpcDefeatedEventDTO::getCode() {
  return static_cast<uint8_t>(ServerOpcode::NPCDefeated);
}

std::unique_ptr<CommandDTO> NpcDefeatedEventDTO::clone() {
  return std::make_unique<NpcDefeatedEventDTO>(*this);
}
