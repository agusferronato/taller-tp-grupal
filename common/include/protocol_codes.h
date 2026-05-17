#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

#include <cstdint>

/*
  Protocol Codes
  Tiene los distintos tipos de mensajes que se pueden enviar entre el
  cliente y el servidor.
*/

// Opcodes para comandos enviados por el cliente al servidor
enum class CommandOpCode : uint8_t {
  RegisterPlayer = 0x10,
  LOGIN_PLAYER,
  MEDITATE,
  PRIVATE_MESSAGE,
  MOVE_COMMAND,
  EXIT,
};

// Opcodes para eventos enviados por el servidor a los clientes
enum class ServerOpcode : uint8_t {
  CHAT_MESSAGE = 0x80,
  NPC_DEFEATED,
  NPC_RESPAWNED,
  PLAYER_MOVED,
};


#endif // PROTOCOL_CODES_H
