#ifndef CITY_ENTITY_COMMAND_PARSER_H
#define CITY_ENTITY_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class CityEntityCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;
  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
