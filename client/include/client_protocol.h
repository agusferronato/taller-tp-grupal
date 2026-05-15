#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../../common/include/binary_protocol.h"
#include "../../common/include/dto/client_command_dto.h"
#include "../../common/include/dto/server_event_dto.h"
#include "../../common/include/protocol_codes.h"
#include "../../common/include/socket.h"
#include <string>

/*
  ClientProtocol
  Maneja la comunicación de su cliente con el servidor.
  Implementa BinaryProtocol para enviar y recibir mensajes usando el protocolo
  definido.

  Ejemplo de uso: client.protocol.send_register_player("Alice");
*/

class ClientProtocol : public BinaryProtocol {
public:
  explicit ClientProtocol(Socket &socket);

  ServerEvent recv_event();
  void send_command(ClientCommand command);

private:
  void send_register_player(const RegisterPlayerCommand &player_name);
  void send_resurrect();
  void send_meditate();
  void send_private_message(const PrivateMessageCommand &private_message);
  void send_exit();
};

#endif // CLIENT_PROTOCOL_H
