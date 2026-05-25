#ifndef PRIVATE_MESSAGE_DTO_H
#define PRIVATE_MESSAGE_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct PrivateMessageDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::PrivateMessage;

public:
  CommandOpCode getCode() const { return opCode; }
  std::string target;
  std::string message;
};

#endif
