#ifndef SERVER_DECODER_H_
#define SERVER_DECODER_H_

#include <memory>

#include "../../common/include/direction.h"
#include "../../common/include/types.h"

/*
    ServerDecoder
    Se encarga de decodificar los comandos recibidos por el servidor.

    Recibe un comando, lee el opcode, y dependiendo del opcode, lee el resto de
   los datos necesarios para crear el comando correspondiente
*/

class Command;
class ServerProtocol;

class ServerDecoder {

private:
  static Direction recv_direction(ServerProtocol &protocol);

public:
  static std::unique_ptr<Command> decode(ServerProtocol &protocol,
                                         PlayerId caller);
};

#endif // SERVER_DECODER_H_
