#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../../common/include/binary_protocol.h"
#include "../../common/include/dto/command_dto.h"
#include "../../common/include/dto/server_event_dto.h"
#include "../../common/include/socket.h"

/*
  ClientProtocol
  Maneja la comunicación de el cliente con el servidor.

  Implementa BinaryProtocol para enviar y recibir mensajes usando el protocolo
  definido.
*/

class ClientDecoder;
class ClientEncoder;

class ClientProtocol : public BinaryProtocol {
  // Para que pueda acceder a los metodos privados/protegidos de ClientProtocol
  friend class ClientDecoder;
  friend class ClientEncoder;

public:
  explicit ClientProtocol(Socket &socket);

  ServerEvent recv_event();
  void send_command(const CommandDTO &command);
};

#endif // CLIENT_PROTOCOL_H
