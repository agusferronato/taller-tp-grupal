#include "parser/Events/TextureInfoEventParser.h"

#include "TextureInfoEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void TextureInfoEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<TextureInfoEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::TextureInfoEvent), bytes);
  utils.appendBytes(event.maxSize, bytes);
  utils.appendBytes(event.gridSize, bytes);
  utils.appendBytes(event.commonGroundTextureId, bytes);
  utils.appendBytes(static_cast<uint16_t>(event.origins.size()), bytes);

  for (const auto &origin : event.origins) {
    utils.appendBytes(origin.priority, bytes);
    utils.appendBytes(origin.texture_id, bytes);
    utils.appendBytes(origin.i, bytes);
    utils.appendBytes(origin.j, bytes);
  }
}

ServerEventDTO TextureInfoEventParser::deserialize(Protocol &protocol) {
  uint16_t maxSize = protocol.getUint16();
  uint16_t gridSize = protocol.getUint16();
  uint16_t commonGroundTextureId = protocol.getUint16();
  uint16_t count = protocol.getUint16();

  std::vector<TextureOriginDTO> origins;
  origins.reserve(count);

  for (uint16_t i = 0; i < count; ++i) {
    uint8_t priority = protocol.getUint8();
    uint16_t texture_id = protocol.getUint16();
    uint16_t ii = protocol.getUint16();
    uint16_t jj = protocol.getUint16();
    origins.push_back({priority, texture_id, ii, jj});
  }

  return TextureInfoEventDTO{maxSize, gridSize, commonGroundTextureId,
                             std::move(origins)};
}
