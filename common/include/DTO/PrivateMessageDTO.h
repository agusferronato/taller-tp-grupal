#ifndef PRIVATE_MESSAGE_DTO_H
#define PRIVATE_MESSAGE_DTO_H

#include "CommandDTO.h"
#include "ProtocolCodes.h"
#include <memory>
#include <string>

class PrivateMessageDTO : public CommandDTO {
private:
  std::string target;
  std::string message;

public:
  PrivateMessageDTO(std::string target, std::string message);

  const std::string &getTarget() const;
  const std::string &getMessage() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
