#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "CommandDTO.h"
#include "Protocol.h"
#include <vector>
#include <memory>
#include "NetworkUtils.h"


class CommandParser {
    
protected:
    NetworkUtils utils;
 
public:
    CommandParser() = default;
 
    virtual void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) = 0;
 
    virtual std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) = 0;
 
    virtual ~CommandParser() = default;


};

#endif