#include "command/CommandFactory.h"

#include "AcceptClanRequestCommandDTO.h"
#include "AtackCommandDTO.h"
#include "BanClanPlayerCommandDTO.h"
#include "CreateClanCommandDTO.h"
#include "AttackCommandDTO.h"
#include "DropItemCommandDTO.h"
#include "EquipCommandDTO.h"
#include "ExitCommandDTO.h"
#include "GlobalChatMessageCommandDTO.h"
#include "JoinClanCommandDTO.h"
#include "KickClanMemberCommandDTO.h"
#include "LeaveClanCommandDTO.h"
#include "LoginPlayerCommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RejectClanRequestCommandDTO.h"
#include "ReviewClanCommandDTO.h"
#include "TakeItemCommandDTO.h"
#include "UnequipCommandDTO.h"
#include "command/AcceptClanRequestCommand.h"
#include "command/AtackCommand.h"
#include "command/BanClanPlayerCommand.h"
#include "command/CreateClanCommand.h"
#include "command/AttackCommand.h"
#include "command/DropItemCommand.h"
#include "command/EquipCommand.h"
#include "command/ExitCommand.h"
#include "command/GlobalChatMessageCommand.h"
#include "command/JoinClanCommand.h"
#include "command/KickClanMemberCommand.h"
#include "command/LeaveClanCommand.h"
#include "command/LoginPlayerCommand.h"
#include "command/MovePlayerCommand.h"
#include "command/PlayerStoppedCommand.h"
#include "command/RegisterPlayerCommand.h"
#include "command/RejectClanRequestCommand.h"
#include "command/ReviewClanCommand.h"
#include "command/TakeItemCommand.h"
#include "command/UnequipCommand.h"

std::unique_ptr<Command> CommandFactory::create(const ClientCommandDTO &dto) {
  if (const auto *request = std::get_if<RegisterPlayerCommandDTO>(&dto)) {
    return std::make_unique<RegisterPlayerCommand>(request->name, request->race,
                                                   request->playerClass);
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
    return std::make_unique<GlobalChatMessageCommand>(request->playerId,
                                                      request->message);
  }

  if (const auto *request = std::get_if<AttackCommandDTO>(&dto)) {
    return std::make_unique<AttackCommand>(request->playerId, request->x,
                                           request->y);
  }

  if (const auto *request = std::get_if<CreateClanCommandDTO>(&dto)) {
    return std::make_unique<CreateClanCommand>(request->playerId,
                                               request->clanName);
  }

  if (const auto *request = std::get_if<JoinClanCommandDTO>(&dto)) {
    return std::make_unique<JoinClanCommand>(request->playerId,
                                             request->clanName);
  }

  if (const auto *request = std::get_if<AcceptClanRequestCommandDTO>(&dto)) {
    return std::make_unique<AcceptClanRequestCommand>(request->founderId,
                                                      request->playerName);
  }

  if (const auto *request = std::get_if<LeaveClanCommandDTO>(&dto)) {
    return std::make_unique<LeaveClanCommand>(request->playerId);
  }

  if (const auto *request = std::get_if<ReviewClanCommandDTO>(&dto)) {
    return std::make_unique<ReviewClanCommand>(request->playerId);
  }

  if (const auto *request = std::get_if<RejectClanRequestCommandDTO>(&dto)) {
    return std::make_unique<RejectClanRequestCommand>(request->founderId,
                                                      request->playerName);
  }

  if (const auto *request = std::get_if<BanClanPlayerCommandDTO>(&dto)) {
    return std::make_unique<BanClanPlayerCommand>(request->founderId,
                                                  request->playerName);
  }

  if (const auto *request = std::get_if<KickClanMemberCommandDTO>(&dto)) {
    return std::make_unique<KickClanMemberCommand>(request->founderId,
                                                   request->playerName);
  }

  throw std::runtime_error("Unknown client request DTO type");
}
