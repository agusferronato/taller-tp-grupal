#include "PlayerStoppedParser.h"

void PlayerStoppedParser::getBytesToSend(std::vector<uint8_t> &bytes, CommandDTO &dto)
{
    auto& stoppedDTO = dynamic_cast<PlayerStoppedDTO&>(dto);
    utils.appendToSend(stoppedDTO.getCode(), bytes);
    utils.appendToSend(stoppedDTO.getPlayerID(), bytes);
}

std::unique_ptr<CommandDTO> PlayerStoppedParser::getDTO(Protocol &protocol)
{
    uint32_t playerID = protocol.getUint32();
    return std::make_unique<PlayerStoppedDTO>(playerID);
}   

