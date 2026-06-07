#ifndef CITY_ENTITY_APPEARED_EVENT_PARSER_H
#define CITY_ENTITY_APPEARED_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class CityEntityAppearedEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;
  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
