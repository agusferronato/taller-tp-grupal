#include "parser/Commands/CreateClanCommandParser.h"

#include "DTO/Commands/CreateClanCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void CreateClanCommandParser::serialize(std::vector<uint8_t> &bytes,
                                        const ClientCommandDTO &dto) {
  const auto &request = std::get<CreateClanCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::CreateClanCommand), bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.clanName, bytes);
}

ClientCommandDTO CreateClanCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  std::string clanName = protocol.getStringData();

  return CreateClanCommandDTO{playerId, std::move(clanName)};
}
