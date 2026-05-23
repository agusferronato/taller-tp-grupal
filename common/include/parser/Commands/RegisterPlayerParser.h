#ifndef REGISTER_PLAYER_PARSER_H
#define REGISTER_PLAYER_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class RegisterPlayerParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
