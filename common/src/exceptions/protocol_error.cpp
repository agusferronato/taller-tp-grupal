#include "protocol_error.h"

ProtocolError::ProtocolError(const std::string &msg) : std::runtime_error(msg) {}
