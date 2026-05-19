#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "socket.h"
#include "CommandParser.h"
#include "CommandDTO.h"
#include "NetworkUtils.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Protocol {

private:
    Socket& socket;
    NetworkUtils utils;
    std::unordered_map<uint8_t, std::unique_ptr<CommandParser>> parsers;
 
public:
    explicit Protocol(Socket& socket);
  
    void registerParser(uint8_t code, std::unique_ptr<CommandParser> parser);
  
    void send(CommandDTO& command);
  
    std::unique_ptr<CommandDTO> receive();
  
    void getStringData(std::string& str);

    uint8_t getUint8();

    uint16_t getUint16();

    int16_t getInt16();

    uint32_t getUint32();
};

#endif /* PROTOCOL_H */