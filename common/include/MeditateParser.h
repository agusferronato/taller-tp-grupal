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
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;
};

#endif
