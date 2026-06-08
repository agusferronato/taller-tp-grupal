#ifndef CITY_ENTITY_MOVED_EVENT_PARSER_H
#define CITY_ENTITY_MOVED_EVENT_PARSER_H

#include "ServerEventParser.h"

class CityEntityMovedEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;
  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
