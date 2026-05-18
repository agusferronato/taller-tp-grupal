#ifndef PLAYER_LIST_PARSER_H
#define PLAYER_LIST_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "PlayerListDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class PlayerListParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;
};

#endif
