#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H


#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include <vector>
#include <string>
#include "RegisterPlayerDTO.h"
#include <memory>
#include <netinet/in.h>
#include "socket.h"
#include <cstring>


class NetworkUtils {
public:
    void appendToSend(uint8_t command, std::vector<uint8_t>& bytes) {
        bytes.push_back(command);
    }
 
    void appendToSend(const std::string& str, std::vector<uint8_t>& bytes) {
        uint16_t len = htons(static_cast<uint16_t>(str.size()));
        uint8_t buf[2];
        memcpy(buf, &len, sizeof(buf));
        bytes.insert(bytes.end(), buf, buf + sizeof(buf));
        bytes.insert(bytes.end(), str.begin(), str.end());
    }
 
    uint8_t receive_uint8(Socket& socket) {
        uint8_t value;
        socket.recvall(&value, sizeof(value));
        return value;
    }
 
    uint16_t receive_uint16(Socket& socket) {
        uint16_t net_value;
        socket.recvall(&net_value, sizeof(net_value));
        return ntohs(net_value);
    }
 
    void recv_string(Socket& socket, std::string& str) {
        uint16_t size = receive_uint16(socket);
        std::string value(size, '\0');
        if (size > 0) {
            socket.recvall(value.data(), size);
        }
        str.append(value);
    }
};


#endif /* NETWORK_UTILS_H */
