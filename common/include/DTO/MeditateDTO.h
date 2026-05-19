#ifndef MEDITATE_DTO_H
#define MEDITATE_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <memory>

class MeditateDTO : public CommandDTO {
public:
  MeditateDTO();

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
