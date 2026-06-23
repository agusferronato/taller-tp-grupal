#ifndef CITY_ENTITY_APPEARED_EVENT_DTO_H
#define CITY_ENTITY_APPEARED_EVENT_DTO_H

#include "Direction.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct CityEntityAppearedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::CityEntityAppearedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t entityId;
  uint8_t type;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif
