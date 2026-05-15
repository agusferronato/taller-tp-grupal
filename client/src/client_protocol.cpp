#include "../include/client_protocol.h"
#include "../../common/include/constants.h"
#include "../../common/include/exceptions/protocol_error.h"
#include <arpa/inet.h>

ClientProtocol::ClientProtocol(Socket &socket) : BinaryProtocol(socket) {}

ServerEvent ClientProtocol::recv_event() {
  auto opcode = static_cast<protocol::ServerOpcode>(recv_uint8());

  switch (opcode) {

  case protocol::ServerOpcode::CHAT_MESSAGE: {
    std::string sender = recv_string();
    std::string msg = recv_string();
    return ChatMessageEvent{std::move(sender), std::move(msg)};
  }

  case protocol::ServerOpcode::NPC_DEFEATED: {
    uint8_t npc_id = recv_uint8();
    return NpcDefeatedEvent{npc_id};
  }

  case protocol::ServerOpcode::PLAYER_MOVED: {
    uint32_t player_id = recv_uint32();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();
    return PlayerMovedEvent{player_id, x, y};
  }

  case protocol::ServerOpcode::ERROR_MESSAGE: {
    std::string error_msg = recv_string();
    throw ProtocolError("Server error: " + error_msg);
  }

  default:
    throw ProtocolError("Unknown server opcode received");
  }
}

void ClientProtocol::send_register_player(const std::string &name) {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::REGISTER_PLAYER));

  send_string(name);
}

void ClientProtocol::send_resurrect() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::RESURRECT));
}

void ClientProtocol::send_meditate() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::MEDITATE));
}

void ClientProtocol::send_private_message(const std::string &target,
                                          const std::string &message) {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::PRIVATE_MESSAGE));

  send_string(target);
  send_string(message);
}

void ClientProtocol::send_exit() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::EXIT));
}
