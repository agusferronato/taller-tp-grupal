#include "ProtocolError.h"

ProtocolError::ProtocolError(const std::string &msg)
    : std::runtime_error(msg) {}
