#ifndef EQUIP_COMMAND_PARSER_H
#define EQUIP_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class EquipCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
