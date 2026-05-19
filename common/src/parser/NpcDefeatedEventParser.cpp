#include "NpcDefeatedEventParser.h"

void NpcDefeatedEventParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                            CommandDTO &dto) {
  auto &eventDTO = dynamic_cast<NpcDefeatedEventDTO &>(dto);
  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::NPCDefeated), bytes);
  utils.appendToSend(eventDTO.getNpcId(), bytes);
}

std::unique_ptr<CommandDTO> NpcDefeatedEventParser::getDTO(Protocol &protocol) {
  uint8_t npc_id = protocol.getUint8();
  return std::make_unique<NpcDefeatedEventDTO>(npc_id);
}
