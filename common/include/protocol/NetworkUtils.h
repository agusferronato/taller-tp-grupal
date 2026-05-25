#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "Socket.h"
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <vector>

class NetworkUtils {
public:
  void appendBytes(uint8_t command, std::vector<uint8_t> &bytes);

  void appendBytes(const std::string &str, std::vector<uint8_t> &bytes);

  void appendBytes(uint16_t value, std::vector<uint8_t> &bytes);

  void appendBytes(int16_t value, std::vector<uint8_t> &bytes);

  void appendBytes(uint32_t value, std::vector<uint8_t> &bytes);

  uint8_t receiveUint8(Socket &socket);

  uint16_t receiveUint16(Socket &socket);

  int16_t receiveInt16(Socket &socket);

  uint32_t receiveInt32(Socket &socket);

  void receiveString(Socket &socket, std::string &str);
};

#endif /* NETWORK_UTILS_H */
