#include "parser/Events/AttackReceivedEventParser.h"
#include "AttackReceivedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void AttackReceivedEventParser::serialize(std::vector<uint8_t> &bytes,
                                          const ServerEventDTO &dto) {
    const auto &event = std::get<AttackReceivedEventDTO>(dto);
    utils.appendBytes(static_cast<uint8_t>(EventOpcode::AttackReceivedEvent),
                      bytes);
    utils.appendBytes(event.damage, bytes);
}

ServerEventDTO AttackReceivedEventParser::deserialize(Protocol &protocol) {
    uint32_t damage = protocol.getUint32();
    return AttackReceivedEventDTO{damage};
}
