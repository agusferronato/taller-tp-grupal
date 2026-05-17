#include "ChatMessageEventParser.h"

void ChatMessageEventParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& eventDTO = dynamic_cast<ChatMessageEventDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(ServerOpcode::CHAT_MESSAGE), bytes);
    utils.appendToSend(eventDTO.getSender(), bytes);
    utils.appendToSend(eventDTO.getMessage(), bytes);
}

std::unique_ptr<CommandDTO> ChatMessageEventParser::getDTO(Protocol& protocol) {
    std::string sender;
    std::string message;
    protocol.getStringData(sender);
    protocol.getStringData(message);
    return std::make_unique<ChatMessageEventDTO>(std::move(sender), std::move(message));
}
