#include "LoginPlayerParser.h"

void LoginPlayerParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                       CommandDTO &dto) {
  auto &loginDTO = std::get<LoginPlayerDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::LoginPlayer), bytes);
  utils.appendToSend(loginDTO.name, bytes);
}

std::unique_ptr<CommandDTO> LoginPlayerParser::getDTO(Protocol &protocol) {
  std::string name;
  protocol.getStringData(name);
  return make_command_dto<LoginPlayerDTO>(std::move(name));
}
