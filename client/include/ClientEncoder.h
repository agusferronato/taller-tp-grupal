#ifndef CLIENT_ENCODER_H
#define CLIENT_ENCODER_H

#include "command_dto.h"
#include "client_protocol.h"

/*
  ClientEncoder
  Codifica los comandos del cliente para enviarlos al servidor.

  Se le pasa un CommandDTO con la informacion del comando, y se encarga de
  enviar el comando correspondiente al servidor
*/

class ClientEncoder {

private:
    ClientProtocol& protocol;

public:
  static void encode(const CommandDTO &cmd);

};

#endif // CLIENT_ENCODER_H
