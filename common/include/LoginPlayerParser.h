#ifndef LOGIN_PLAYER_PARSER_H
#define LOGIN_PLAYER_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "LoginPlayerDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <string>
#include <memory>

class LoginPlayerParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        auto& loginDTO = dynamic_cast<LoginPlayerDTO&>(dto);
        utils.appendToSend(static_cast<uint8_t>(CommandOpCode::LOGIN_PLAYER), bytes);
        utils.appendToSend(loginDTO.getName(), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        std::string name;
        protocol.getStringData(name);
        return std::make_unique<LoginPlayerDTO>(std::move(name));
    }
};

#endif
