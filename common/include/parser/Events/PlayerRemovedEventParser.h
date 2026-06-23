#ifndef PLAYER_REMOVED_EVENT_PARSER_H
#define PLAYER_REMOVED_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class PlayerRemovedEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
