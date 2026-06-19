#include "protocol/ServerEventCodes.h"

EventOpcode getCode(const ServerEventDTO &event) {
  return std::visit(
      [](const auto &concreteEvent) { return concreteEvent.getCode(); }, event);
}
