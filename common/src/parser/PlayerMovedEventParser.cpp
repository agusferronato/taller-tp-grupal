#include "PlayerMovedEventParser.h"
#include "ProtocolError.h"

void PlayerMovedEventParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                            CommandDTO &dto) {
  auto &eventDTO = std::get<PlayerMovedEventDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PlayerMoved), bytes);
  utils.appendToSend(eventDTO.player_id, bytes);
  utils.appendToSend(eventDTO.x, bytes);
  utils.appendToSend(eventDTO.y, bytes);
  bytes.push_back(static_cast<uint8_t>(eventDTO.direction));
}

std::unique_ptr<CommandDTO> PlayerMovedEventParser::getDTO(Protocol &protocol) {
  uint32_t player_id = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  uint8_t raw_dir = protocol.getUint8();
  switch (static_cast<Direction>(raw_dir)) {
  case Direction::Up:
  case Direction::Down:
  case Direction::Left:
  case Direction::Right:
    return make_command_dto<PlayerMovedEventDTO>(
        player_id, x, y, static_cast<Direction>(raw_dir));
  default:
    throw ProtocolError("Invalid direction in PlayerMovedEvent");
  }
}
