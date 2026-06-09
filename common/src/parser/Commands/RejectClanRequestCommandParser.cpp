#include "parser/Commands/RejectClanRequestCommandParser.h"

#include "DTO/Commands/RejectClanRequestCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void RejectClanRequestCommandParser::serialize(std::vector<uint8_t> &bytes,
                                               const ClientCommandDTO &dto) {
  const auto &request = std::get<RejectClanRequestCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::RejectClanRequestCommand),
      bytes);
  utils.appendBytes(request.founderId, bytes);
  utils.appendBytes(request.playerName, bytes);
}

ClientCommandDTO
RejectClanRequestCommandParser::deserialize(Protocol &protocol) {
  uint32_t founderId = protocol.getUint32();
  std::string playerName = protocol.getStringData();

  return RejectClanRequestCommandDTO{founderId, std::move(playerName)};
}
