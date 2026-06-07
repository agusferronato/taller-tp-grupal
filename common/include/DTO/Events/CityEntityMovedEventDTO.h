#ifndef CITY_ENTITY_MOVED_EVENT_DTO_H
#define CITY_ENTITY_MOVED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include "Direction.h"
#include <cstdint>

struct CityEntityMovedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::CityEntityMovedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t entityId;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif
