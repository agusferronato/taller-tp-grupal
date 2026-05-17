#ifndef CLIENT_DECODER_H
#define CLIENT_DECODER_H

#include "../../common/include/dto/server_event_dto.h"

/*
    ClientDecoder
    Se encarga de decodificar los eventos recibidos por el cliente.

    Lee el opcode enviado por el servidor y, dependiendo del opcode,
    lee el resto de los datos necesarios para crear el ServerEvent
   correspondiente.
*/

class Command;
class ClientProtocol;

class ClientDecoder {
public:
  static ServerEvent decode(ClientProtocol &protocol);
};

#endif // CLIENT_DECODER_H_
