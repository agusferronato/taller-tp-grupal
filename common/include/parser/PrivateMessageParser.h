#ifndef PRIVATE_MESSAGE_PARSER_H
#define PRIVATE_MESSAGE_PARSER_H

#include "CommandDTO.h"
#include "CommandParser.h"
#include "PrivateMessageDTO.h"
#include "Protocol.h"
#include "protocol_codes.h"
#include <memory>
#include <string>
#include <vector>

class PrivateMessageParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
