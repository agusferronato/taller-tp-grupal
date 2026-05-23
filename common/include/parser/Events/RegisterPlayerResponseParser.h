#ifndef REGISTER_PLAYER_RESPONSE_PARSER_H
#define REGISTER_PLAYER_RESPONSE_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class RegisterPlayerResponseParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
