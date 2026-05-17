#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "socket.h"
#include "CommandParser.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include <vector>
#include <string>
#include "RegisterPlayerDTO.h"
#include <memory>
#include "NetworkUtils.h"
#include <unordered_map>

class Protocol {

private:
    Socket& socket;
    NetworkUtils utils;
    std::unordered_map<uint8_t, std::unique_ptr<CommandParser>> parsers;
 
public:
    explicit Protocol(Socket& socket) : socket(socket) {}
 
    void registerParser(uint8_t code, std::unique_ptr<CommandParser> parser) {
        parsers[code] = std::move(parser);
    }
 
    void send(CommandDTO& command) {

        std::vector<uint8_t> bytes;
        uint8_t code = command.getCode();
        auto it = parsers.find(code);

        it->second->getBytesToSend(bytes, command);
        if (!bytes.empty())
            socket.sendall(bytes.data(), bytes.size());
    }
 
    std::unique_ptr<CommandDTO> receive() {

        uint8_t code = utils.receive_uint8(socket);
        auto it = parsers.find(code);

        return it->second->getDTO(*this);
    }
 
    void getStringData(std::string& str) {
        utils.recv_string(socket, str);
    }
};

#endif /* PROTOCOL_H */