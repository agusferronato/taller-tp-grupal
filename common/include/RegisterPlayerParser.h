#ifndef REGISTER_PLAYER_PARSER_H
#define REGISTER_PLAYER_PARSER_H


#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include <vector>
#include <string>
#include "RegisterPlayerDTO.h"
#include <memory>
#include "protocol_codes.h"


class RegisterPlayerParser : public CommandParser {

public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {

        auto& registerDTO = dynamic_cast<RegisterPlayerDTO&>(dto);
        std::string name = registerDTO.getName();
        utils.appendToSend(static_cast<uint8_t>(CommandOpCode::RegisterPlayer), bytes);
        utils.appendToSend(name, bytes);

    }
 
    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {

        std::string name;
        protocol.getStringData(name);
        return std::make_unique<RegisterPlayerDTO>(std::move(name));

    }
};


#endif