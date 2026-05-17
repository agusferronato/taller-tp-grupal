#ifndef CHAT_MESSAGE_EVENT_PARSER_H
#define CHAT_MESSAGE_EVENT_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "ChatMessageEventDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <string>
#include <memory>

class ChatMessageEventParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        auto& eventDTO = dynamic_cast<ChatMessageEventDTO&>(dto);
        utils.appendToSend(static_cast<uint8_t>(ServerOpcode::CHAT_MESSAGE), bytes);
        utils.appendToSend(eventDTO.getSender(), bytes);
        utils.appendToSend(eventDTO.getMessage(), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        std::string sender;
        std::string message;
        protocol.getStringData(sender);
        protocol.getStringData(message);
        return std::make_unique<ChatMessageEventDTO>(std::move(sender), std::move(message));
    }
};

#endif
