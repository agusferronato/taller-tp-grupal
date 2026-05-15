#include "../include/server_protocol.h"
#include "../../common/include/constants.h"
#include "../../common/include/exceptions/protocol_error.h"
#include <arpa/inet.h>

ServerProtocol::ServerProtocol(Socket &socket) : BinaryProtocol(socket) {}

ClientCommand ServerProtocol::recv_command() {
  auto opcode = static_cast<protocol::ClientOpcode>(recv_uint8());

  switch (opcode) {
  case protocol::ClientOpcode::REGISTER_PLAYER:
    return RegisterPlayerCommand{recv_string()};

  case protocol::ClientOpcode::RESURRECT:
    return ResurrectCommand{};

  case protocol::ClientOpcode::MEDITATE:
    return MeditateCommand{};

  case protocol::ClientOpcode::PRIVATE_MESSAGE: {
    std::string target = recv_string();
    std::string msg = recv_string();
    return PrivateMessageCommand{std::move(target), std::move(msg)};
  }

  case protocol::ClientOpcode::EXIT:
    return ExitCommand{};

  default:
    throw ProtocolError("Unknown client opcode received");
  }
}

void ServerProtocol::send_chat_message(const std::string &sender,
                                       const std::string &message) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::CHAT_MESSAGE));
  send_string(sender);
  send_string(message);
}

void ServerProtocol::send_error_message(const std::string &message) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::ERROR_MESSAGE));
  send_string(message);
}

void ServerProtocol::send_npc_defeated(uint8_t npc_id) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::NPC_DEFEATED));
  send_uint8(npc_id);
}

void ServerProtocol::send_npc_respawned(uint8_t npc_id) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::NPC_RESPAWNED));
  send_uint8(npc_id);
}

void ServerProtocol::send_player_moved(uint32_t player_id, uint16_t x,
                                       uint16_t y) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::PLAYER_MOVED));
  send_uint32(player_id);
  send_uint16(x);
  send_uint16(y);
}
