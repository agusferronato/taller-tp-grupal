#include "ChatMessageEventDTO.h"

ChatMessageEventDTO::ChatMessageEventDTO(std::string sender, std::string message)
    : sender(std::move(sender)), message(std::move(message)) {}

const std::string& ChatMessageEventDTO::getSender() const { return sender; }

const std::string& ChatMessageEventDTO::getMessage() const { return message; }

uint8_t ChatMessageEventDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::ChatMessage);
}

std::unique_ptr<CommandDTO> ChatMessageEventDTO::clone() {
    return std::make_unique<ChatMessageEventDTO>(*this);
}
