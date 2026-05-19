#ifndef CHAT_MESSAGE_EVENT_PARSER_H
#define CHAT_MESSAGE_EVENT_PARSER_H

#include "ChatMessageEventDTO.h"
#include "CommandDTO.h"
#include "CommandParser.h"
#include "Protocol.h"
#include "protocol_codes.h"
#include <memory>
#include <string>
#include <vector>

class ChatMessageEventParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
