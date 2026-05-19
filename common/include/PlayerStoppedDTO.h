#ifndef PLAYER_STOPPED_DTO_H
#define PLAYER_STOPPED_DTO_H


#include <cstdint>
#include "CommandDTO.h"
#include "protocol_codes.h"

class PlayerStoppedDTO : public CommandDTO {


private:

    uint32_t playerID;

public:

    PlayerStoppedDTO(uint32_t playerID);

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;

    uint32_t getPlayerID();

};





#endif
