#ifndef PLAYER_STOP_COMMAND_PARSER_H
#define PLAYER_STOP_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class PlayerStopCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
