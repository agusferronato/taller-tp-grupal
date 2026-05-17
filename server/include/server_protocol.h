#ifndef _SERVER_PROTOCOL_H_
#define _SERVER_PROTOCOL_H_

#include "../../common/include/binary_protocol.h"
#include "../../common/include/dto/client_command_dto.h"
#include "../../common/include/dto/server_event_dto.h"
#include "../../common/include/protocol_codes.h"
#include "../../common/include/socket.h"
#include <string>

/*
  ServerProtocol
  Maneja la comunicación del servidor con los clientes.
  Implementa BinaryProtocol para enviar y recibir mensajes usando el protocolo
  definido.

  Ejemplo de uso: server_protocol.send_player_moved(1, 10, 20);
*/

class ServerProtocol : public BinaryProtocol {
public:
  explicit ServerProtocol(Socket &socket);

  ClientCommand recv_command();
  void send_event(const ServerEvent &event);

private:
  void send_chat_message(const ChatMessageEvent &chat_msg);

  void send_npc_defeated(const NpcDefeatedEvent &npc_defeated);

  void send_player_moved(const PlayerMovedEvent &player_moved);
};

#endif // _SERVER_PROTOCOL_H_
