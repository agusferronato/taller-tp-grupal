#ifndef EXIT_PARSER_H
#define EXIT_PARSER_H

#include "CommandDTO.h"
#include "CommandParser.h"
#include "ExitDTO.h"
#include "Protocol.h"
#include "protocol_codes.h"
#include <memory>
#include <vector>

class ExitParser : public CommandParser {
public:
  void getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) override;

  std::unique_ptr<CommandDTO> getDTO(Protocol &protocol) override;
};

#endif
