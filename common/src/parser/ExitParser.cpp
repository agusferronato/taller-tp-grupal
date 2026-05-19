#include "ExitParser.h"

void ExitParser::getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto) {
  (void)dto;
  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::Exit), bytes);
}

std::unique_ptr<CommandDTO> ExitParser::getDTO(Protocol &protocol) {
  (void)protocol;
  return std::make_unique<ExitDTO>();
}
