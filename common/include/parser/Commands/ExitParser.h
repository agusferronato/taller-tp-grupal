#ifndef EXIT_COMMAND_PARSER_H
#define EXIT_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class ExitParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
