#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "DTO/Commands/ClientRequestDTO.h"
#include "DTO/Events/EventDTO.h"

#include "parser/Commands/ClientRequestParser.h"
#include "parser/Events/ServerEventParser.h"

#include "Socket.h"
#include "protocol/NetworkUtils.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Protocol {
private:
  Socket &socket;
  NetworkUtils utils;

  std::unordered_map<uint8_t, std::unique_ptr<ClientRequestParser>>
      commandParsers;
  std::unordered_map<uint8_t, std::unique_ptr<ServerEventParser>> eventParsers;

public:
  explicit Protocol(Socket &socket);

  void registerCommandParser(uint8_t code,
                             std::unique_ptr<ClientRequestParser> parser);

  void registerEventParser(uint8_t code,
                           std::unique_ptr<ServerEventParser> parser);

  void sendCommand(const ClientRequestDTO &command);
  ClientRequestDTO receiveCommand();

  void sendEvent(const ServerEventDTO &event);
  ServerEventDTO receiveEvent();

  std::string getStringData();

  uint8_t getUint8();
  uint16_t getUint16();
  int16_t getInt16();
  uint32_t getUint32();
};

#endif
