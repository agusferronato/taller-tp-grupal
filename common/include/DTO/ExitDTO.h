#ifndef EXIT_DTO_H
#define EXIT_DTO_H

#include "CommandDTO.h"
#include "ProtocolCodes.h"
#include <memory>

class ExitDTO : public CommandDTO {
public:
  ExitDTO();

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
