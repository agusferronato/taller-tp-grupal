#ifndef REGISTER_PLAYER_RESPONSE_DTO_H
#define REGISTER_PLAYER_RESPONSE_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include "types.h"

class RegisterPlayerResponseDTO : public CommandDTO {
private:
    PlayerId player_id;
    uint8_t status;

public:
    RegisterPlayerResponseDTO(PlayerId player_id, uint8_t status);

    PlayerId getPlayerId() const;
    uint8_t getStatus() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;
};

#endif
