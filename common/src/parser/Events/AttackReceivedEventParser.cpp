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
    utils.appendBytes(static_cast<uint8_t>(event.entityType), bytes);
    utils.appendBytes(event.entityId, bytes);
    utils.appendBytes(static_cast<uint8_t>(event.effectType), bytes);
}

ServerEventDTO AttackReceivedEventParser::deserialize(Protocol &protocol) {
    EntityType entityType = static_cast<EntityType>(protocol.getUint8());
    uint32_t entityId = protocol.getUint32();
    EffectType effectType = static_cast<EffectType>(protocol.getUint8());
    return AttackReceivedEventDTO{entityType, entityId, effectType};
}
