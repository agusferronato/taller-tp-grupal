#include "command/CommandFactory.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"


std::unique_ptr<Command> CommandFactory::create(std::unique_ptr<CommandDTO> dto) {
    const uint8_t code = dto->getCode();

    if (code == static_cast<uint8_t>(CommandOpCode::RegisterPlayer)) {
        return std::make_unique<RegisterPlayerCommand>();
    }

    if (code == static_cast<uint8_t>(CommandOpCode::MoveCommand)) {
        auto& moveDto = dynamic_cast<MoveCommandDTO&>(*dto);

        return std::make_unique<MovePlayerCommand>(
            moveDto.getPlayerId(),
            moveDto.getDirection()
        );
    }

    if (code == static_cast<uint8_t>(ServerOpcode::PlayerStopped)) {
        auto& stoppedDto = dynamic_cast<PlayerStoppedDTO&>(*dto);

        return std::make_unique<PlayerStoppedCommand>(
            stoppedDto.getPlayerID()
        );
    }

    throw std::runtime_error("Unknown command DTO opcode");
}
