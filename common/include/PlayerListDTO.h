#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include "types.h"
#include <vector>
#include <cstdint>

class PlayerListDTO : public CommandDTO {
private:
    std::vector<PlayerId> player_ids;

public:
    explicit PlayerListDTO(std::vector<PlayerId> player_ids);

    const std::vector<PlayerId>& getPlayerIds() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;
};

#endif
