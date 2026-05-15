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
    throw ProtocolError(
        "Unable to process Command: Unknown client opcode received");
  }
}

void ServerProtocol::send_event(const ServerEvent &event) {
  if (std::holds_alternative<ChatMessageEvent>(event)) {
    send_chat_message(std::get<ChatMessageEvent>(event));

  } else if (std::holds_alternative<NpcDefeatedEvent>(event)) {
    send_npc_defeated(std::get<NpcDefeatedEvent>(event));

  } else if (std::holds_alternative<PlayerMovedEvent>(event)) {
    send_player_moved(std::get<PlayerMovedEvent>(event));

  } else {
    throw ProtocolError("Unable to send event: Unknown server event");
  }
}

void ServerProtocol::send_chat_message(const ChatMessageEvent &chat_msg) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::CHAT_MESSAGE));
  send_string(chat_msg.sender);
  send_string(chat_msg.message);
}

void ServerProtocol::send_npc_defeated(const NpcDefeatedEvent &npc_defeated) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::NPC_DEFEATED));
  send_uint8(npc_defeated.npc_id);
}

void ServerProtocol::send_player_moved(const PlayerMovedEvent &player_moved) {
  send_uint8(static_cast<uint8_t>(protocol::ServerOpcode::PLAYER_MOVED));
  send_uint32(player_moved.player_id);
  send_uint16(player_moved.x);
  send_uint16(player_moved.y);
}
