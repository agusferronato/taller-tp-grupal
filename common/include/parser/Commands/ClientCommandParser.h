#ifndef CLIENT_COMMAND_PARSER_H
#define CLIENT_COMMAND_PARSER_H

#include "DTO/Commands/ClientCommandDTO.h"
#include "protocol/NetworkUtils.h"

#include <cstdint>
#include <vector>

class Protocol;

class ClientCommandParser {
protected:
  NetworkUtils utils;

public:
  virtual ~ClientCommandParser() = default;

  virtual void serialize(std::vector<uint8_t> &bytes,
                         const ClientCommandDTO &dto) = 0;

  virtual ClientCommandDTO deserialize(Protocol &protocol) = 0;
};

#endif
