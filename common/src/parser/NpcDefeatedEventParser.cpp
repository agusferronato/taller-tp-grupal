#include "NpcDefeatedEventParser.h"

void NpcDefeatedEventParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                            CommandDTO &dto) {
  auto &eventDTO = std::get<NpcDefeatedEventDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::NPCDefeated), bytes);
  utils.appendToSend(eventDTO.npc_id, bytes);
}

std::unique_ptr<CommandDTO> NpcDefeatedEventParser::getDTO(Protocol &protocol) {
  uint8_t npc_id = protocol.getUint8();
  return make_command_dto<NpcDefeatedEventDTO>(npc_id);
}
