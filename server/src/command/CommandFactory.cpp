#include "command/CommandFactory.h"

#include "ExitDTO.h"
#include "LoginPlayerDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopDTO.h"
#include "RegisterPlayerDTO.h"
#include "command/ExitCommand.h"
#include "command/LoginPlayerCommand.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"

std::unique_ptr<Command> CommandFactory::create(const ClientRequestDTO &dto) {
  if (const auto *request = std::get_if<RegisterPlayerDTO>(&dto)) {
    return std::make_unique<RegisterPlayerCommand>(request->name);
  }

  if (const auto *request = std::get_if<LoginPlayerDTO>(&dto)) {
    return std::make_unique<LoginPlayerCommand>(request->name);
  }

  if (const auto *request = std::get_if<MoveCommandDTO>(&dto)) {
    return std::make_unique<MovePlayerCommand>(request->player_id,
                                               request->direction);
  }

  if (const auto *request = std::get_if<PlayerStopDTO>(&dto)) {
    return std::make_unique<PlayerStoppedCommand>(request->player_id);
  }

  if (const auto *request = std::get_if<ExitDTO>(&dto)) {
    return std::make_unique<ExitCommand>(request->playerId);
  }

  throw std::runtime_error("Unknown client request DTO type");
}
