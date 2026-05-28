#ifndef PLAYER_INFO_EVENT_PARSER_H
#define PLAYER_INFO_EVENT_PARSER_H

#include "ServerEventParser.h"
#include <vector>

class PlayerInfoEventParser : public ServerEventParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ServerEventDTO &dto) override;

  ServerEventDTO deserialize(Protocol &protocol) override;
};

#endif
