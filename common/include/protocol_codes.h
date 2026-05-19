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
  LoginPlayer,
  Meditate,
  PrivateMessage,
  MoveCommand,
  Exit,
};

// Opcodes para eventos enviados por el servidor a los clientes
enum class ServerOpcode : uint8_t {
  ChatMessage = 0x80,
  NPCDefeated,
  NPCRespawned,
  PlayerMoved,
  RegisterResponse,
  PlayerList,
  PlayerAppeared,
  PlayerStopped,
};


#endif // PROTOCOL_CODES_H
