#include "parser/Commands/MeditateParser.h"

#include "DTO/Commands/MeditateDTO.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void MeditateParser::serialize(std::vector<uint8_t> &bytes,
                               const ClientRequestDTO &dto) {
  std::get<MeditateDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::Meditate), bytes);
}

ClientRequestDTO MeditateParser::deserialize(Protocol &) {
  return MeditateDTO{};
}
