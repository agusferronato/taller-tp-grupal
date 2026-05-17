#include "../include/server_protocol.h"

#include "../../common/include/command/command.h"
#include "../include/server_decoder.h"
#include "../include/server_encoder.h"

ServerProtocol::ServerProtocol(Socket &socket) : BinaryProtocol(socket) {}

// Devuelve el comando recibido ya parseado
std::unique_ptr<Command> ServerProtocol::recv_command(PlayerId caller) {
  return ServerDecoder::decode(*this, caller);
}

// Envia el evento dado al cliente usando el protocolo definido
void ServerProtocol::send_event(const ServerEvent &event) {
  ServerEncoder::encode(*this, event);
}
