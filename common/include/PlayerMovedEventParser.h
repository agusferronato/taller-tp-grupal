#ifndef PLAYER_MOVED_EVENT_PARSER_H
#define PLAYER_MOVED_EVENT_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "PlayerMovedEventDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <memory>

class PlayerMovedEventParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;
};

#endif
