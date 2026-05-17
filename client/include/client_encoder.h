#ifndef CLIENT_ENCODER_H
#define CLIENT_ENCODER_H

#include "../../common/include/dto/command_dto.h"

/*
  ClientEncoder
  Codifica los comandos del cliente para enviarlos al servidor.

  Se le pasa un CommandDTO con la informacion del comando, y se encarga de
  enviar el comando correspondiente al servidor
*/

class ClientProtocol;

class ClientEncoder {
private:
  static void send(ClientProtocol &protocol, const RegisterPlayerDTO &cmd);
  static void send(ClientProtocol &protocol, const PrivateMessageDTO &cmd);

public:
  static void encode(ClientProtocol &protocol, const CommandDTO &cmd);
};

#endif // CLIENT_ENCODER_H
