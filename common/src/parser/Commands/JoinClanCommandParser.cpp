#include "parser/Commands/JoinClanCommandParser.h"

#include "DTO/Commands/JoinClanCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void JoinClanCommandParser::serialize(std::vector<uint8_t> &bytes,
                                      const ClientCommandDTO &dto) {
  const auto &request = std::get<JoinClanCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::JoinClanCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.clanName, bytes);
}

ClientCommandDTO JoinClanCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  std::string clanName = protocol.getStringData();

  return JoinClanCommandDTO{playerId, std::move(clanName)};
}
