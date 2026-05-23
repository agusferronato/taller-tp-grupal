#include "MoveCommandParser.h"

void MoveCommandParser::getBytesToSend(std::vector<uint8_t> &bytes,
                                       CommandDTO &dto) {
  auto &moveDTO = std::get<MoveCommandDTO>(dto);
  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::MoveCommand), bytes);
  utils.appendToSend(moveDTO.player_id, bytes);
  bytes.push_back(static_cast<uint8_t>(moveDTO.direction));
}

std::unique_ptr<CommandDTO> MoveCommandParser::getDTO(Protocol &protocol) {
  uint32_t player_id = protocol.getUint32();
  uint8_t raw = protocol.getUint8();
  switch (static_cast<Direction>(raw)) {
  case Direction::Up:
  case Direction::Down:
  case Direction::Left:
  case Direction::Right:
    return make_command_dto<MoveCommandDTO>(player_id,
                                            static_cast<Direction>(raw));
  default:
    throw ProtocolError("Invalid direction received");
  }
}
