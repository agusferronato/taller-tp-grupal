#ifndef LOGIN_PLAYER_PARSER_H
#define LOGIN_PLAYER_PARSER_H

#include "CommandDTO.h"
#include "CommandParser.h"
#include "LoginPlayerDTO.h"
#include "Protocol.h"
#include "ProtocolCodes.h"
#include <memory>
#include <string>
#include <vector>

class LoginPlayerParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
