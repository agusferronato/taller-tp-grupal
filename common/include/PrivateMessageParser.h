#ifndef PRIVATE_MESSAGE_PARSER_H
#define PRIVATE_MESSAGE_PARSER_H

#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "PrivateMessageDTO.h"
#include "protocol_codes.h"
#include <vector>
#include <string>
#include <memory>

class PrivateMessageParser : public CommandParser {
public:
    void getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) override;

    std::unique_ptr<CommandDTO> getDTO(Protocol& protocol) override;
};

#endif
