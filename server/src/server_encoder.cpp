#include "../include/server_encoder.h"

#include "../../common/include/exceptions/protocol_error.h"
#include "../../common/include/protocol_codes.h"
#include "../include/server_protocol.h"

void ServerEncoder::encode(ServerProtocol &protocol, const ServerEvent &event) {
  if (std::holds_alternative<ChatMessageEvent>(event)) {
    send(protocol, std::get<ChatMessageEvent>(event));

  } else if (std::holds_alternative<NpcDefeatedEvent>(event)) {
    send(protocol, std::get<NpcDefeatedEvent>(event));

  } else if (std::holds_alternative<PlayerMovedEvent>(event)) {
    send(protocol, std::get<PlayerMovedEvent>(event));

  } else {
    throw ProtocolError("Unable to send event: Unknown server event");
  }
}

void ServerEncoder::send(ServerProtocol &protocol,
                         const ChatMessageEvent &event) {
  protocol.send_uint8(
      static_cast<uint8_t>(protocol::ServerOpcode::CHAT_MESSAGE));
  protocol.send_string(event.sender);
  protocol.send_string(event.message);
}

void ServerEncoder::send(ServerProtocol &protocol,
                         const NpcDefeatedEvent &event) {
  protocol.send_uint8(
      static_cast<uint8_t>(protocol::ServerOpcode::NPC_DEFEATED));
  protocol.send_uint8(event.npc_id);
}

void ServerEncoder::send(ServerProtocol &protocol,
                         const PlayerMovedEvent &event) {
  protocol.send_uint8(
      static_cast<uint8_t>(protocol::ServerOpcode::PLAYER_MOVED));
  protocol.send_uint32(event.player_id);
  protocol.send_uint16(event.x);
  protocol.send_uint16(event.y);
}
