#ifndef SERVER_ENCODER_H_
#define SERVER_ENCODER_H_

#include "../../common/include/dto/server_event_dto.h"

/*
  ServerEncoder
  Se encarga de codificar los eventos del servidor y enviarlos a los clientes.
*/

class ServerProtocol;

class ServerEncoder {
private:
  static void send(ServerProtocol &protocol, const ChatMessageEvent &event);
  static void send(ServerProtocol &protocol, const NpcDefeatedEvent &event);
  static void send(ServerProtocol &protocol, const PlayerMovedEvent &event);

public:
  static void encode(ServerProtocol &protocol, const ServerEvent &event);
};

#endif // SERVER_ENCODER_H_
