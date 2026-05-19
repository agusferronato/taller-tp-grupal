#ifndef LOGIN_PLAYER_DTO_H
#define LOGIN_PLAYER_DTO_H

#include "CommandDTO.h"
#include "ProtocolCodes.h"
#include <memory>
#include <string>

class LoginPlayerDTO : public CommandDTO {
private:
  std::string name;

public:
  explicit LoginPlayerDTO(std::string name);

  const std::string &getName() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
