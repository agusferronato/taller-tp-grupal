#include "command/CommandFactory.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"

std::unique_ptr<Command>
CommandFactory::create(std::unique_ptr<CommandDTO> dto) {
  const uint8_t code = get_command_code(*dto);

  if (code == static_cast<uint8_t>(CommandOpCode::RegisterPlayer)) {
    return std::make_unique<RegisterPlayerCommand>();
  }

  if (code == static_cast<uint8_t>(CommandOpCode::MoveCommand)) {
    auto &moveDto = std::get<MoveCommandDTO>(*dto);

    return std::make_unique<MovePlayerCommand>(moveDto.player_id,
                                               moveDto.direction);
  }

  if (code == static_cast<uint8_t>(ServerOpcode::PlayerStopped)) {
    auto &stoppedDto = std::get<PlayerStoppedDTO>(*dto);

    return std::make_unique<PlayerStoppedCommand>(stoppedDto.player_id);
  }

  throw std::runtime_error("Unknown command DTO opcode");
}
