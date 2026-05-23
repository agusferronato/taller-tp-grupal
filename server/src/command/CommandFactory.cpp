#include "command/CommandFactory.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"

std::unique_ptr<Command> CommandFactory::create(const ClientRequestDTO &dto) {
  if ([[maybe_unused]] const auto *request =
          std::get_if<RegisterPlayerDTO>(&dto)) {
    return std::make_unique<RegisterPlayerCommand>();
  }

  if (const auto *request = std::get_if<MoveCommandDTO>(&dto)) {
    return std::make_unique<MovePlayerCommand>(request->player_id,
                                               request->direction);
  }

  if (const auto *request = std::get_if<PlayerStopDTO>(&dto)) {
    return std::make_unique<PlayerStoppedCommand>(request->player_id);
  }

  throw std::runtime_error("Unknown client request DTO type");
}
