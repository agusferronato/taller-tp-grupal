#ifndef MOVE_COMMAND_PARSER_H
#define MOVE_COMMAND_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "MoveCommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"
#include "exceptions/protocol_error.h"
#include <vector>
#include <memory>

class MoveCommandParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;
};

#endif
