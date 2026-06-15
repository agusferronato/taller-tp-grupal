#include "parser/Commands/CityEntityCommandParser.h"

#include "CityEntityCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <variant>

void CityEntityCommandParser::serialize(std::vector<uint8_t> &bytes,
                                        const ClientCommandDTO &dto) {
  const auto &cmd = std::get<CityEntityCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::CityEntityCommand),
                    bytes);
  utils.appendBytes(cmd.playerId, bytes);
  utils.appendBytes(cmd.type, bytes);
  utils.appendBytes(cmd.arg, bytes);
}

ClientCommandDTO CityEntityCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t type = protocol.getUint8();
  std::string arg = protocol.getStringData();
  return CityEntityCommandDTO{playerId, type, std::move(arg)};
}
