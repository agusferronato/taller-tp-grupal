#ifndef MEDITATE_PARSER_H
#define MEDITATE_PARSER_H

#include "ClientRequestParser.h"
#include <vector>

class MeditateParser : public ClientRequestParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientRequestDTO &dto) override;

  ClientRequestDTO deserialize(Protocol &protocol) override;
};

#endif
