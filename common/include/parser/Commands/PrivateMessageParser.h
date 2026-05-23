#ifndef PRIVATE_MESSAGE_PARSER_H
#define PRIVATE_MESSAGE_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class PrivateMessageParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
