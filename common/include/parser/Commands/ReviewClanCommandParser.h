#ifndef REVIEW_CLAN_COMMAND_PARSER_H
#define REVIEW_CLAN_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class ReviewClanCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
