#include "command/CommandFactory.h"

#include "DropItemCommandDTO.h"
#include "EquipCommandDTO.h"
#include "ExitCommandDTO.h"
#include "LoginPlayerCommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "TakeItemCommandDTO.h"
#include "UnequipCommandDTO.h"
#include "GlobalChatMessageCommandDTO.h"
#include "command/DropItemCommand.h"
#include "command/EquipCommand.h"
#include "command/ExitCommand.h"
#include "command/LoginPlayerCommand.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"
#include "command/TakeItemCommand.h"
#include "command/UnequipCommand.h"
#include "command/GlobalChatMessageCommand.h"

std::unique_ptr<Command> CommandFactory::create(const ClientCommandDTO &dto) {
  if (const auto *request =
          std::get_if<RegisterPlayerCommandDTO>(&dto)) {
    return std::make_unique<RegisterPlayerCommand>(request->playerName, request->race, request->playerClass);
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

  if (const auto *request = std::get_if<EquipCommandDTO>(&dto)) {
    return std::make_unique<EquipCommand>(request->playerId,
                                          request->inventorySlot);
  }

  if (const auto *request = std::get_if<UnequipCommandDTO>(&dto)) {
    return std::make_unique<UnequipCommand>(request->playerId,
                                            request->equipSlot);
  }

  if (const auto *request = std::get_if<DropItemCommandDTO>(&dto)) {
    return std::make_unique<DropItemCommand>(request->playerId,
                                             request->inventorySlot);
  }

  if (const auto *request = std::get_if<TakeItemCommandDTO>(&dto)) {
    return std::make_unique<TakeItemCommand>(request->playerId);
  }

  if (const auto *request = std::get_if<GlobalChatMessageCommandDTO>(&dto)) {
    return std::make_unique<GlobalChatMessageCommand>(request->playerId, request->message);
  }

  throw std::runtime_error("Unknown client request DTO type");
}
