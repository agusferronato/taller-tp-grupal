#ifndef LOGIN_PLAYER_PARSER_H
#define LOGIN_PLAYER_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class LoginPlayerParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
