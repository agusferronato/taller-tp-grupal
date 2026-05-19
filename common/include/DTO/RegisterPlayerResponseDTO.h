#ifndef REGISTER_PLAYER_RESPONSE_DTO_H
#define REGISTER_PLAYER_RESPONSE_DTO_H

#include "CommandDTO.h"
#include "ProtocolCodes.h"
#include <memory>

class RegisterPlayerResponseDTO : public CommandDTO {
private:
  uint32_t player_id;
  uint8_t status;

public:
  RegisterPlayerResponseDTO(uint32_t player_id, uint8_t status);

  uint32_t getPlayerId() const;
  uint8_t getStatus() const;

  uint8_t getCode() override;
  std::unique_ptr<CommandDTO> clone() override;
};

#endif
