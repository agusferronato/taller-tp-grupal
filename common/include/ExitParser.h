#ifndef EXIT_PARSER_H
#define EXIT_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "ExitDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class ExitParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override {
        (void)dto;
        utils.appendToSend(static_cast<uint8_t>(CommandOpCode::EXIT), bytes);
    }

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override {
        (void)protocol;
        return std::make_unique<ExitDTO>();
    }
};

#endif
