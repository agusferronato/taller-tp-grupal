#ifndef PLAYER_STOP_COMMAND_PARSER_H
#define PLAYER_STOP_COMMAND_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class PlayerStopCommandParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
