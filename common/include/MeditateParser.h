#ifndef MEDITATE_PARSER_H
#define MEDITATE_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "MeditateDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class MeditateParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        (void)dto;
        utils.appendToSend(static_cast<uint8_t>(CommandOpCode::MEDITATE), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        (void)protocol;
        return std::make_unique<MeditateDTO>();
    }
};

#endif
