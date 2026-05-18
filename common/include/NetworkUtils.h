#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "socket.h"
#include <vector>
#include <string>
#include <netinet/in.h>
#include <cstring>

class NetworkUtils {
public:
    void appendToSend(uint8_t command, std::vector<uint8_t>& bytes);
  
    void appendToSend(const std::string& str, std::vector<uint8_t>& bytes);

    void appendToSend(uint16_t value, std::vector<uint8_t>& bytes);

    void appendToSend(int16_t value, std::vector<uint8_t>& bytes);

    void appendToSend(uint32_t value, std::vector<uint8_t>& bytes);
   
    uint8_t receive_uint8(Socket& socket);
   
    uint16_t receive_uint16(Socket& socket);

    int16_t receive_int16(Socket& socket);

    uint32_t receive_uint32(Socket& socket);
   
    void recv_string(Socket& socket, std::string& str);
};


#endif /* NETWORK_UTILS_H */
