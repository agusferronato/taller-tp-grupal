#ifndef _PROTOCOL_ERROR_H_
#define _PROTOCOL_ERROR_H_

#include <stdexcept>
#include <string>

class ProtocolError : public std::runtime_error {
public:
  explicit ProtocolError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
