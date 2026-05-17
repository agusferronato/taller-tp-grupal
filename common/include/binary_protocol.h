#ifndef BINARY_PROTOCOL_H
#define BINARY_PROTOCOL_H

#include "../../common/include/socket.h"
#include <cstdint>
#include <string>

/*
  BinaryProtocol
  Protocolo de comunicación binario para enviar y recibir datos entre el cliente
  y el servidor.
  Proporciona metodos para enviar y recibir tipos de datos básicos (uint8,
  uint16, uint32, string) siguiendo un formato especifico.
*/

class BinaryProtocol {

protected:
  Socket &socket;

  explicit BinaryProtocol(Socket &socket);

  void send_uint8(uint8_t value);
  uint8_t recv_uint8();

  void send_uint16(uint16_t value);
  uint16_t recv_uint16();

  void send_uint32(uint32_t value);
  uint32_t recv_uint32();

  void send_string(const std::string &value);
  std::string recv_string();
};

#endif // BINARY_PROTOCOL_H
