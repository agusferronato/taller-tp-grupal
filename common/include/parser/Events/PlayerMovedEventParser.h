#ifndef PLAYER_MOVED_EVENT_PARSER_H
#define PLAYER_MOVED_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class PlayerMovedEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
