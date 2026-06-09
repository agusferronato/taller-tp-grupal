#include "parser/Commands/AcceptClanRequestCommandParser.h"

#include "DTO/Commands/AcceptClanRequestCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void AcceptClanRequestCommandParser::serialize(std::vector<uint8_t> &bytes,
                                               const ClientCommandDTO &dto) {
  const auto &request = std::get<AcceptClanRequestCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::AcceptClanRequestCommand),
      bytes);
  utils.appendBytes(request.founderId, bytes);
  utils.appendBytes(request.playerName, bytes);
}

ClientCommandDTO
AcceptClanRequestCommandParser::deserialize(Protocol &protocol) {
  uint32_t founderId = protocol.getUint32();
  std::string playerName = protocol.getStringData();

  return AcceptClanRequestCommandDTO{founderId, std::move(playerName)};
}
