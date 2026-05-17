#ifndef BINARY_PROTOCOL_H
#define BINARY_PROTOCOL_H

#include "../../common/include/socket.h"
#include <cstdint>
#include <string>

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
