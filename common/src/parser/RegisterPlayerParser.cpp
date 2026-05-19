#include "RegisterPlayerParser.h"

void RegisterPlayerParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                          CommandDTO &dto) {
  auto &registerDTO = dynamic_cast<RegisterPlayerDTO &>(dto);
  std::string name = registerDTO.getName();
  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
                     bytes);
  utils.appendToSend(name, bytes);
}

std::unique_ptr<CommandDTO> RegisterPlayerParser::getDTO(Protocol &protocol) {
  std::string name;
  protocol.getStringData(name);
  return std::make_unique<RegisterPlayerDTO>(std::move(name));
}
