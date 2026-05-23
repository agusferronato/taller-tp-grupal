#include "ChatMessageEventParser.h"

void ChatMessageEventParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                            CommandDTO &dto) {
  auto &eventDTO = std::get<ChatMessageEventDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::ChatMessage), bytes);
  utils.appendToSend(eventDTO.sender, bytes);
  utils.appendToSend(eventDTO.message, bytes);
}

std::unique_ptr<CommandDTO> ChatMessageEventParser::getDTO(Protocol &protocol) {
  std::string sender;
  std::string message;
  protocol.getStringData(sender);
  protocol.getStringData(message);
  return make_command_dto<ChatMessageEventDTO>(std::move(sender),
                                               std::move(message));
}
