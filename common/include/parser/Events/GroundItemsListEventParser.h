#ifndef GROUND_ITEMS_LIST_EVENT_PARSER_H
#define GROUND_ITEMS_LIST_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class GroundItemsListEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
