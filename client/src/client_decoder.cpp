#include "../include/client_decoder.h"
#include "../../common/include/exceptions/protocol_error.h"
#include "../../common/include/protocol_codes.h"
#include "../include/client_protocol.h"

ServerEvent ClientDecoder::decode(ClientProtocol &protocol) {
  auto opcode = static_cast<protocol::ServerOpcode>(protocol.recv_uint8());

  switch (opcode) {

  case protocol::ServerOpcode::CHAT_MESSAGE: {
    std::string sender = protocol.recv_string();
    std::string message = protocol.recv_string();

    return ChatMessageEvent{std::move(sender), std::move(message)};
  }

  case protocol::ServerOpcode::NPC_DEFEATED: {
    uint8_t npc_id = protocol.recv_uint8();

    return NpcDefeatedEvent{npc_id};
  }

  case protocol::ServerOpcode::PLAYER_MOVED: {
    uint32_t player_id = protocol.recv_uint32();
    uint16_t x = protocol.recv_uint16();
    uint16_t y = protocol.recv_uint16();

    return PlayerMovedEvent{player_id, x, y};
  }

  default:
    throw ProtocolError("Unable to decode event: Unknown server opcode");
  }
}
