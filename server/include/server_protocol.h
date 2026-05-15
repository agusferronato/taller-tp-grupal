#ifndef _SERVER_PROTOCOL_H_
#define _SERVER_PROTOCOL_H_

#include "../../common/include/binary_protocol.h"
#include "../../common/include/client_command_dto.h"
#include "../../common/include/protocol_codes.h"
#include "../../common/include/server_event_dto.h"
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

  void send_chat_message(const std::string &sender, const std::string &message);

  void send_error_message(const std::string &message);

  void send_npc_defeated(uint8_t npc_id);

  void send_npc_respawned(uint8_t npc_id);

  void send_player_moved(uint32_t player_id, uint16_t x, uint16_t y);
};

#endif // _SERVER_PROTOCOL_H_
