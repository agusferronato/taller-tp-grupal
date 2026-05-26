#ifndef PRIVATE_MESSAGE_COMMAND_PARSER_H
#define PRIVATE_MESSAGE_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class PrivateMessageParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
