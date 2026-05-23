#ifndef MOVE_COMMAND_PARSER_H
#define MOVE_COMMAND_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class MoveCommandParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
