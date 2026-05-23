#include "PlayerStoppedParser.h"

void PlayerStoppedParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                         CommandDTO &dto) {
  auto &stoppedDTO = std::get<PlayerStoppedDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PlayerStopped), bytes);
  utils.appendToSend(stoppedDTO.player_id, bytes);
}

std::unique_ptr<CommandDTO> PlayerStoppedParser::getDTO(Protocol &protocol) {
  uint32_t playerID = protocol.getUint32();
  return make_command_dto<PlayerStoppedDTO>(playerID);
}
