#include "ExitDTO.h"

ExitDTO::ExitDTO() {}

uint8_t ExitDTO::getCode() { return static_cast<uint8_t>(CommandOpCode::Exit); }

std::unique_ptr<CommandDTO> ExitDTO::clone() {
  return std::make_unique<ExitDTO>(*this);
}
