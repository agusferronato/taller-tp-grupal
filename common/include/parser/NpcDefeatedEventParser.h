#ifndef NPC_DEFEATED_EVENT_PARSER_H
#define NPC_DEFEATED_EVENT_PARSER_H

#include "CommandDTO.h"
#include "CommandParser.h"
#include "NpcDefeatedEventDTO.h"
#include "Protocol.h"
#include "ProtocolCodes.h"
#include <memory>
#include <vector>

class NpcDefeatedEventParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
