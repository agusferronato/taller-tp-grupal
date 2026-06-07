#ifndef CITY_ENTITY_STOPPED_EVENT_DTO_H
#define CITY_ENTITY_STOPPED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct CityEntityStoppedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::CityEntityStoppedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t entityId;
};

#endif
