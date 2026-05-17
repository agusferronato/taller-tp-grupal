#include "../include/binary_protocol.h"
#include "../../common/include/exceptions/protocol_error.h"
#include "../include/constants.h"
#include <arpa/inet.h>

BinaryProtocol::BinaryProtocol(Socket &socket) : socket(socket) {}

void BinaryProtocol::send_uint8(uint8_t value) {
  socket.sendall(&value, sizeof(value));
}

uint8_t BinaryProtocol::recv_uint8() {
  uint8_t value;
  socket.recvall(&value, sizeof(value));
  return value;
}

void BinaryProtocol::send_uint16(uint16_t value) {
  uint16_t net_value = htons(value);
  socket.sendall(&net_value, sizeof(net_value));
}

uint16_t BinaryProtocol::recv_uint16() {
  uint16_t net_value;
  socket.recvall(&net_value, sizeof(net_value));
  return ntohs(net_value);
}

void BinaryProtocol::send_uint32(uint32_t value) {
  uint32_t net_value = htonl(value);
  socket.sendall(&net_value, sizeof(net_value));
}

uint32_t BinaryProtocol::recv_uint32() {
  uint32_t net_value;
  socket.recvall(&net_value, sizeof(net_value));
  return ntohl(net_value);
}

void BinaryProtocol::send_string(const std::string &value) {
  if (value.size() > protocol::MAX_STRING_SIZE) {
    throw ProtocolError("String too long to send");
  }

  send_uint16(static_cast<uint16_t>(value.size()));

  if (!value.empty()) {
    socket.sendall(value.data(), value.size());
  }
}

std::string BinaryProtocol::recv_string() {
  uint16_t size = recv_uint16();

  if (size > protocol::MAX_STRING_SIZE) {
    throw ProtocolError("Received string too long");
  }

  std::string value(size, '\0');

  if (size > 0) {
    socket.recvall(value.data(), size);
  }

  return value;
}
