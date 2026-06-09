#include "parser/Commands/KickClanMemberCommandParser.h"

#include "DTO/Commands/KickClanMemberCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void KickClanMemberCommandParser::serialize(std::vector<uint8_t> &bytes,
                                            const ClientCommandDTO &dto) {
  const auto &request = std::get<KickClanMemberCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::KickClanMemberCommand), bytes);
  utils.appendBytes(request.founderId, bytes);
  utils.appendBytes(request.playerName, bytes);
}

ClientCommandDTO KickClanMemberCommandParser::deserialize(Protocol &protocol) {
  uint32_t founderId = protocol.getUint32();
  std::string playerName = protocol.getStringData();

  return KickClanMemberCommandDTO{founderId, std::move(playerName)};
}
