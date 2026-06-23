#ifndef TEXTURE_INFO_EVENT_DTO_H
#define TEXTURE_INFO_EVENT_DTO_H

#include "MapData.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <vector>

struct TextureInfoEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::TextureInfoEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint16_t maxSize;
  uint16_t gridSize;
  uint16_t commonGroundTextureId;
  std::vector<TextureOriginDTO> origins;
};

#endif
