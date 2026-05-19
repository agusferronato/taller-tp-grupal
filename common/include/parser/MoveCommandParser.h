#ifndef MOVE_COMMAND_PARSER_H
#define MOVE_COMMAND_PARSER_H

#include "CommandDTO.h"
#include "CommandParser.h"
#include "Direction.h"
#include "MoveCommandDTO.h"
#include "Protocol.h"
#include "ProtocolCodes.h"
#include "ProtocolError.h"
#include <memory>
#include <vector>

class MoveCommandParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
