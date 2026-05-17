#include "../include/server_decoder.h"

#include "../../common/include/command/move_command.h"
#include "../../common/include/command/private_message_command.h"
#include "../../common/include/command/register_player_command.h"
#include "../../common/include/exceptions/protocol_error.h"
#include "../../common/include/protocol_codes.h"
#include "../include/server_protocol.h"

std::unique_ptr<Command> ServerDecoder::decode(ServerProtocol &protocol,
                                               PlayerId caller) {
  auto opcode = static_cast<protocol::ClientOpcode>(protocol.recv_uint8());

  switch (opcode) {

  case protocol::ClientOpcode::REGISTER_PLAYER: {
    std::string name = protocol.recv_string();
    return std::make_unique<RegisterPlayerCommand>(std::move(name));
  }

  case protocol::ClientOpcode::MOVE_COMMAND: {
    Direction direction = recv_direction(protocol);
    return std::make_unique<MoveCommand>(caller, direction);
  }

  case protocol::ClientOpcode::PRIVATE_MESSAGE: {
    std::string target = protocol.recv_string();
    std::string msg = protocol.recv_string();
    return std::make_unique<PrivateMessageCommand>(caller, std::move(target),
                                                   std::move(msg));
  }

  default:
    throw ProtocolError("Unknown client opcode received");
  }
}

Direction ServerDecoder::recv_direction(ServerProtocol &protocol) {
  uint8_t raw = protocol.recv_uint8();

  switch (static_cast<Direction>(raw)) {
  case Direction::UP:
  case Direction::DOWN:
  case Direction::LEFT:
  case Direction::RIGHT:
    return static_cast<Direction>(raw);

  default:
    throw ProtocolError("Invalid direction received");
  }
}
