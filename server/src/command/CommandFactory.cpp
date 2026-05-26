#include "command/CommandFactory.h"

#include "ExitCommandDTO.h"
#include "LoginPlayerCommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "command/ExitCommand.h"
#include "command/LoginPlayerCommand.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"

std::unique_ptr<Command> CommandFactory::create(const ClientCommandDTO &dto) {
  if (const auto *request =
          std::get_if<RegisterPlayerCommandDTO>(&dto)) {
    return std::make_unique<RegisterPlayerCommand>(request->name);
  }

  if (const auto *request = std::get_if<LoginPlayerCommandDTO>(&dto)) {
    return std::make_unique<LoginPlayerCommand>(request->name);
  }

  if (const auto *request = std::get_if<MoveCommandDTO>(&dto)) {
    return std::make_unique<MovePlayerCommand>(request->playerId,
                                                request->direction);
  }

  if (const auto *request = std::get_if<PlayerStopCommandDTO>(&dto)) {
    return std::make_unique<PlayerStoppedCommand>(request->playerId);
  }

  if (const auto *request = std::get_if<ExitCommandDTO>(&dto)) {
    return std::make_unique<ExitCommand>(request->playerId);
  }

  throw std::runtime_error("Unknown client request DTO type");
}
