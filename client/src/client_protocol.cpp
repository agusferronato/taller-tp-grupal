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

  default:
    throw ProtocolError("Unknown server opcode received");
  }
}

void ClientProtocol::send_command(ClientCommand command) {
  if (std::holds_alternative<RegisterPlayerCommand>(command)) {
    send_register_player(std::get<RegisterPlayerCommand>(command));

  } else if (std::holds_alternative<ResurrectCommand>(command)) {
    send_resurrect();

  } else if (std::holds_alternative<MeditateCommand>(command)) {
    send_meditate();

  } else if (std::holds_alternative<PrivateMessageCommand>(command)) {
    const auto &pm_cmd = std::get<PrivateMessageCommand>(command);
    send_private_message(pm_cmd);

  } else if (std::holds_alternative<ExitCommand>(command)) {
    send_exit();

  } else {
    throw ProtocolError("Unable to send command: Unknown client command");
  }
}

void ClientProtocol::send_register_player(const RegisterPlayerCommand &player_name) {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::REGISTER_PLAYER));

  send_string(player_name.name);
}

void ClientProtocol::send_resurrect() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::RESURRECT));
}

void ClientProtocol::send_meditate() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::MEDITATE));
}

void ClientProtocol::send_private_message(const PrivateMessageCommand &private_message) {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::PRIVATE_MESSAGE));

  send_string(private_message.target);
  send_string(private_message.message);
}

void ClientProtocol::send_exit() {
  send_uint8(static_cast<uint8_t>(protocol::ClientOpcode::EXIT));
}
