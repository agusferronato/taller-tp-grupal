#include "../include/client_protocol.h"
#include "../include/client_decoder.h"
#include "../include/client_encoder.h"

ClientProtocol::ClientProtocol(Socket &socket) : BinaryProtocol(socket) {}

// Devuelve el evento recibido ya parseado
ServerEvent ClientProtocol::recv_event() {
  return ClientDecoder::decode(*this);
}

// Envia el comando al servidor
void ClientProtocol::send_command(const CommandDTO &command) {
  ClientEncoder::encode(*this, command);
}
