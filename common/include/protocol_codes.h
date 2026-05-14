#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

#include <cstdint>

/*
    Protocol Codes
    Tiene los distintos tipos de mensajes que se pueden enviar entre el
    cliente y el servidor.
*/

namespace protocol {

enum class ClientOpcode : uint8_t {
  REGISTER_PLAYER = 0x10,
  RESURRECT,
  MEDITATE,
  PRIVATE_MESSAGE,
  EXIT,
};

enum class ServerOpcode : uint8_t {
  CHAT_MESSAGE = 0x80,
  ERROR_MESSAGE,
  NPC_DEFEATED,
  NPC_RESPAWNED,
};

} // namespace protocol

#endif // PROTOCOL_CODES_H
