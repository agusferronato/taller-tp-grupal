#include "PrivateMessageParser.h"

void PrivateMessageParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                          CommandDTO &dto) {
  auto &pmDTO = std::get<PrivateMessageDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::PrivateMessage),
                     bytes);
  utils.appendToSend(pmDTO.target, bytes);
  utils.appendToSend(pmDTO.message, bytes);
}

std::unique_ptr<CommandDTO> PrivateMessageParser::getDTO(Protocol &protocol) {
  std::string target;
  std::string message;
  protocol.getStringData(target);
  protocol.getStringData(message);
  return make_command_dto<PrivateMessageDTO>(std::move(target),
                                             std::move(message));
}
