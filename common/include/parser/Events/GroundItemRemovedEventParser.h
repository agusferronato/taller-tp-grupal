#ifndef GROUND_ITEM_REMOVED_EVENT_PARSER_H
#define GROUND_ITEM_REMOVED_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class GroundItemRemovedEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
