#ifndef CHAT_MESSAGE_EVENT_DTO_H
#define CHAT_MESSAGE_EVENT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <string>

class ChatMessageEventDTO : public CommandDTO {
private:
    std::string sender;
    std::string message;

public:
    ChatMessageEventDTO(std::string sender, std::string message)
        : sender(std::move(sender)), message(std::move(message)) {}

    const std::string& getSender() const { return sender; }
    const std::string& getMessage() const { return message; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(ServerOpcode::CHAT_MESSAGE);
    }
};

#endif
