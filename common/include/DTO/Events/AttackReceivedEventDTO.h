#ifndef ATTACK_RECEIVED_EVENT_DTO_H
#define ATTACK_RECEIVED_EVENT_DTO_H

#include "EffectType.h"
#include "EntityType.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct AttackReceivedEventDTO {
  static constexpr EventOpcode opCode = EventOpcode::AttackReceivedEvent;
  EventOpcode getCode() const { return opCode; }
  EntityType entityType;
  uint32_t entityId;
  EffectType effectType;
};

#endif
