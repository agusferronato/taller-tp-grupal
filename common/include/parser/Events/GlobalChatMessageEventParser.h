#ifndef GLOBAL_CHAT_MESSAGE_EVENT_PARSER_H
#define GLOBAL_CHAT_MESSAGE_EVENT_PARSER_H

#include "parser/Events/ServerEventParser.h"
#include <vector>

class GlobalChatMessageEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
