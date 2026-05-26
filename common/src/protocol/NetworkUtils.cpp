#include "protocol/NetworkUtils.h"

void NetworkUtils::appendBytes(uint8_t command, std::vector<uint8_t> &bytes) {
  bytes.push_back(command);
}

void NetworkUtils::appendBytes(const std::string &str,
                               std::vector<uint8_t> &bytes) {
  uint16_t len = htons(static_cast<uint16_t>(str.size()));
  uint8_t buf[2];
  memcpy(buf, &len, sizeof(buf));
  bytes.insert(bytes.end(), buf, buf + sizeof(buf));
  bytes.insert(bytes.end(), str.begin(), str.end());
}

void NetworkUtils::appendBytes(uint16_t value, std::vector<uint8_t> &bytes) {
  uint16_t net = htons(value);
  uint8_t buf[2];
  memcpy(buf, &net, sizeof(buf));
  bytes.insert(bytes.end(), buf, buf + sizeof(buf));
}

void NetworkUtils::appendBytes(int16_t value, std::vector<uint8_t> &bytes) {
  appendBytes(static_cast<uint16_t>(value), bytes);
}

void NetworkUtils::appendBytes(uint32_t value, std::vector<uint8_t> &bytes) {
  uint32_t net = htonl(value);
  uint8_t buf[4];
  memcpy(buf, &net, sizeof(buf));
  bytes.insert(bytes.end(), buf, buf + sizeof(buf));
}

uint8_t NetworkUtils::receiveUint8(Socket &socket) {
  uint8_t value;
  socket.recvall(&value, sizeof(value));
  return value;
}

uint16_t NetworkUtils::receiveUint16(Socket &socket) {
  uint16_t net_value;
  socket.recvall(&net_value, sizeof(net_value));
  return ntohs(net_value);
}

int16_t NetworkUtils::receiveInt16(Socket &socket) {
  return static_cast<int16_t>(receiveUint16(socket));
}

uint32_t NetworkUtils::receiveInt32(Socket &socket) {
  uint32_t net_value;
  socket.recvall(&net_value, sizeof(net_value));
  return ntohl(net_value);
}

void NetworkUtils::receiveString(Socket &socket, std::string &str) {
  uint16_t size = receiveUint16(socket);
  std::string value(size, '\0');
  if (size > 0) {
    socket.recvall(value.data(), size);
  }
  str.append(value);
}
