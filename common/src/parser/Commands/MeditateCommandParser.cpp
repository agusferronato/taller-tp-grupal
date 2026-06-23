#include "parser/Commands/MeditateParser.h"

#include "DTO/Commands/MeditateCommandDTO.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void MeditateParser::serialize(std::vector<uint8_t> &bytes,
                               const ClientCommandDTO &dto) {
  std::get<MeditateCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::MeditateCommand),
                    bytes);
}

ClientCommandDTO MeditateParser::deserialize(Protocol &) {
  return MeditateCommandDTO{};
}
