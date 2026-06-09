#include "parser/Commands/BanClanPlayerCommandParser.h"

#include "DTO/Commands/BanClanPlayerCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void BanClanPlayerCommandParser::serialize(std::vector<uint8_t> &bytes,
                                           const ClientCommandDTO &dto) {
  const auto &request = std::get<BanClanPlayerCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::BanClanPlayerCommand), bytes);
  utils.appendBytes(request.founderId, bytes);
  utils.appendBytes(request.playerName, bytes);
}

ClientCommandDTO BanClanPlayerCommandParser::deserialize(Protocol &protocol) {
  uint32_t founderId = protocol.getUint32();
  std::string playerName = protocol.getStringData();

  return BanClanPlayerCommandDTO{founderId, std::move(playerName)};
}
