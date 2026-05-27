#ifndef SERVER_EVENT_PARSER_H
#define SERVER_EVENT_PARSER_H

#include "DTO/Events/EventDTO.h"
#include "protocol/NetworkUtils.h"

#include <cstdint>
#include <vector>

class Protocol;

class ServerEventParser {
protected:
  NetworkUtils utils;

public:
  virtual ~ServerEventParser() = default;

  virtual void serialize(std::vector<uint8_t> &bytes,
                         const ServerEventDTO &dto) = 0;

  virtual ServerEventDTO deserialize(Protocol &protocol) = 0;
};

#endif
