#ifndef PLAYER_STOPPED_PARSER_H
#define PLAYER_STOPPED_PARSER_H

#include "CommandParser.h"
#include "Protocol.h"
#include <memory>
#include "PlayerStoppedDTO.h"


class PlayerStoppedParser : public CommandParser {

public:

    PlayerStoppedParser() : CommandParser() { }

    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;
 
    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;


};


#endif