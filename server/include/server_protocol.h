#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include "../../common/include/binary_protocol.h"
#include "../../common/include/dto/server_event_dto.h"
#include "../../common/include/types.h"
#include <memory>

/*
  ServerProtocol
  Maneja la comunicación del servidor con los clientes.
  Implementa BinaryProtocol para enviar y recibir mensajes.
*/

class Command;
class ServerDecoder;
class ServerEncoder;

class ServerProtocol : public BinaryProtocol {
  // Para que pueda acceder a los metodos privados/protegidos de ServerProtocol
  friend class ServerDecoder;
  friend class ServerEncoder;

public:
  explicit ServerProtocol(Socket &socket);

  // Smart pointer ya que Command es una clase abstracta y no se puede
  // instanciar directamente
  std::unique_ptr<Command> recv_command(PlayerId caller);
  void send_event(const ServerEvent &event);
};

#endif // SERVER_PROTOCOL_H_
