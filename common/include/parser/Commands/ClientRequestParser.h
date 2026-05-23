#ifndef CLIENT_REQUEST_PARSER_H
#define CLIENT_REQUEST_PARSER_H

#include "DTO/Commands/ClientRequestDTO.h"
#include "protocol/NetworkUtils.h"

#include <cstdint>
#include <vector>

class Protocol;

class ClientRequestParser {
protected:
  NetworkUtils utils;

public:
  virtual ~ClientRequestParser() = default;

  virtual void serialize(std::vector<uint8_t> &bytes,
                         const ClientRequestDTO &dto) = 0;

  virtual ClientRequestDTO deserialize(Protocol &protocol) = 0;
};

#endif
