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
    ChatMessageEventDTO(std::string sender, std::string message);

    const std::string& getSender() const;
    const std::string& getMessage() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;
};

#endif
