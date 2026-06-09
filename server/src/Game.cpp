#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <sstream>

#include "CityEntityAppearedEventDTO.h"
#include "CityEntityMovedEventDTO.h"
#include "CityEntityStoppedEventDTO.h"
#include "ConstantRateLoop.h"
#include "Formulas.h"
#include "Game.h"
#include "GlobalChatMessageEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "LoginResultEventDTO.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "NPCAppearedEventDTO.h"
#include "NPCMovedEventDTO.h"
#include "NPCStoppedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerResurrectEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStopCommandDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include "command/CommandFactory.h"
#include "GroundItemsListEventDTO.h"
#include "AttackReceivedEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "GlobalChatMessageEventDTO.h"
#include <NPCType.h>

namespace {
ChatMessageEventDTO makeChatMessage(ChatMessageCategory category,
                                    std::string sender,
                                    std::string message) {
  return ChatMessageEventDTO{category, std::move(sender), std::move(message)};
}

ChatMessageEventDTO makeClanMessage(std::string message) {
  return makeChatMessage(ChatMessageCategory::Clan, "Clan",
                         std::move(message));
}

ChatMessageEventDTO makeClanErrorMessage(std::string message) {
  return makeChatMessage(ChatMessageCategory::Error, "Clan",
                         std::move(message));
}

ChatMessageEventDTO makeSystemErrorMessage(std::string message) {
  return makeChatMessage(ChatMessageCategory::Error, "Sistema",
                         std::move(message));
}

ChatMessageEventDTO makeSystemMessage(std::string message) {
  return makeChatMessage(ChatMessageCategory::System, "Sistema",
                         std::move(message));
}

ChatMessageEventDTO makeCombatMessage(std::string message) {
  return makeChatMessage(ChatMessageCategory::Combat, "Sistema",
                         std::move(message));
}
} // namespace

Game::Game(Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository,
           ClanManager &clanManager, const std::string &mapPath)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor),
      repository(repository), clanManager(clanManager), mapPath(mapPath) {}

void Game::run() {
  MapLoader mapLoader(mapPath);
  maxSize = mapLoader.GetMaxSize();
  gridSize = mapLoader.GetGridSize();
  commonGroundTextureId = mapLoader.GetCommonGroundTextureId();
  textureOrigins = mapLoader.GetTextureOrigins();
  collidableCells = mapLoader.GetCollidableCells();

  biomes = std::move(mapLoader.GetBiomes());
  cities = std::move(mapLoader.GetCities());
  createCityEntities();

  for (uint8_t id = 1; id <= 19; ++id) {
    inventoryManager.addGroundItem(id, (id - 1) * 64, 32);
  }

  ConstantRateLoop rateloop(FPS_SERVER);
  CommandFactory factory;
  unsigned int it = 0;
  unsigned int saveCounter = 0;

  while (keepRunning) {
    ClientMessage msg;

    if (gameloopQueue.try_pop(msg)) {
      auto command = factory.create(msg.dto);
      command->execute(*this, msg.connectionId);
    }

    makeNPCsfollowPlayers();
    makeCitiesEntitiesFollowPlayers();
    movePlayers();

    appearNPCs();
    updateResurrectingPlayers();
    sendMessages();

    if (++saveCounter >= 300) {
      for (auto &[id, player] : players) {
        repository.save(player->getName(), player->toPlayerData());
      }
      clanManager.persist();
      saveCounter = 0;
    }

    if (it % 60 == 0) {
      for (auto &[pid, p] : players) {
        messagesToSend.push_back(p->toPlayerInfoEvent());
      }
    }

    rateloop.updateTimer(it);
  }

  saveAllPlayers();
  clanManager.persist();
}

void Game::kill() { keepRunning = false; }

void Game::registerPlayer(const std::string &name, const Race race,
                          const PlayerClass playerClass,
                          uint32_t connectionId) {

  if (repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId,
        RegisterPlayerEventDTO{0, RegisterStatus::PlayerAlreadyExists});
    return;
  }

  uint32_t newId = connectionId;
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 64;

  auto player = std::make_unique<Character>(newId, name, race, playerClass,
                                             spawnX, spawnY, Direction::Down);
  player->addItem(17);
  player->addItem(1);
  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  playerIdByName[name] = newId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId,
                                                         RegisterStatus::Success});

  {
    std::vector<TextureOriginDTO> origins;
    origins.reserve(textureOrigins.size());
    for (const auto &o : textureOrigins) {
      origins.push_back(
          {static_cast<uint8_t>(o.priority), static_cast<uint16_t>(o.texture_id),
           static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId,
        TextureInfoEventDTO{
            static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
            static_cast<uint16_t>(commonGroundTextureId), std::move(origins)});
  }

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(info->toPlayerInfo(pid));
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  for (auto &npc : npcs) {
    senderQueueMonitor.sendToClient(
        connectionId,
        NPCAppearedEventDTO{npc->getId(), static_cast<uint8_t>(npc->getType()),
                            static_cast<int16_t>(npc->getX()),
                            static_cast<int16_t>(npc->getY())});
  }

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      senderQueueMonitor.sendToClient(
          connectionId,
          CityEntityAppearedEventDTO{
              entity->getId(),
              static_cast<uint8_t>(entity->getCityEntityType()),
              static_cast<int16_t>(entity->getX()),
              static_cast<int16_t>(entity->getY()), entity->getDirection()});
    }
  }

  messagesToSend.push_back(players[newId]->toPlayerAppeared());

  messagesToSend.push_back(InventoryUpdateEventDTO{
      newId, players[newId]->getInventoryItems(),
      players[newId]->getEquippedWeapon(), players[newId]->getEquippedArmor(),
      players[newId]->getEquippedHelmet(),
      players[newId]->getEquippedShield()});

  {
    std::vector<GroundItemInfoDTO> groundItemList;
    for (const auto &gi : inventoryManager.getGroundItems()) {
      groundItemList.push_back({gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                static_cast<int16_t>(gi.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId, GroundItemsListEventDTO{std::move(groundItemList)});
  }
}

void Game::validateLogin(const std::string &name, uint32_t connectionId) {
  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::PlayerNotFound});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->getName() == name) {
      senderQueueMonitor.sendToClient(
          connectionId, LoginResultEventDTO{0, LoginStatus::AlreadyOnline});
      return;
    }
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  LoginResultEventDTO{0, LoginStatus::Success});
}

void Game::loginPlayer(const std::string &name, uint32_t connectionId) {

  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::PlayerNotFound});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->getName() == name) {
      senderQueueMonitor.sendToClient(
          connectionId, LoginResultEventDTO{0, LoginStatus::AlreadyOnline});
      return;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = connectionId;

  auto player = std::make_unique<Character>(newId, data);
  uint32_t savedClanId = player->getClanId();
  if (savedClanId != 0) {
    const Clan *clan = clanManager.getClan(savedClanId);
    if (clan == nullptr || !clan->hasMember(player->getName())) {
      player->leaveClan();
      repository.save(player->getName(), player->toPlayerData());
    }
  }

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  playerIdByName[name] = newId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  LoginResultEventDTO{newId,
                                                      LoginStatus::Success});

  std::vector<TextureOriginDTO> origins;
  origins.reserve(textureOrigins.size());
  for (const auto &o : textureOrigins) {
    origins.push_back({static_cast<uint8_t>(o.priority),
                       static_cast<uint16_t>(o.texture_id),
                       static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
  }
  senderQueueMonitor.sendToClient(
      connectionId,
      TextureInfoEventDTO{
          static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
           static_cast<uint16_t>(commonGroundTextureId), std::move(origins)});

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(info->toPlayerInfo(pid));
  }
  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  for (auto &npc : npcs) {
    senderQueueMonitor.sendToClient(
        connectionId,
        NPCAppearedEventDTO{npc->getId(), static_cast<uint8_t>(npc->getType()),
                            static_cast<int16_t>(npc->getX()),
                            static_cast<int16_t>(npc->getY())});
  }

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      senderQueueMonitor.sendToClient(
          connectionId,
          CityEntityAppearedEventDTO{
              entity->getId(),
              static_cast<uint8_t>(entity->getCityEntityType()),
              static_cast<int16_t>(entity->getX()),
              static_cast<int16_t>(entity->getY()), entity->getDirection()});
    }
  }

  messagesToSend.push_back(players[newId]->toPlayerAppeared());

  if (players[newId]->isDead())
    messagesToSend.push_back(PlayerDieEventDTO{players[newId]->getId()});

  messagesToSend.push_back(InventoryUpdateEventDTO{
      newId, players[newId]->getInventoryItems(),
      players[newId]->getEquippedWeapon(), players[newId]->getEquippedArmor(),
      players[newId]->getEquippedHelmet(),
      players[newId]->getEquippedShield()});

  {
    std::vector<GroundItemInfoDTO> groundItemList;
    for (const auto &gi : inventoryManager.getGroundItems()) {
      groundItemList.push_back({gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                static_cast<int16_t>(gi.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId, GroundItemsListEventDTO{std::move(groundItemList)});
  }

  uint32_t clanId = players[newId]->getClanId();
  if (clanId != 0) {
    sendToClan(clanId, makeClanMessage(name + " entro a Argentum"), newId);
  }
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end() || isResurrecting(playerId)) {
    return;
  }

  it->second->setDirection(direction);
}

void Game::stopPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  it->second->stop();
  messagesToSend.push_back(PlayerStoppedEventDTO{playerId});
}

void Game::exitPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }

  const std::string playerName = it->second->getName();
  uint32_t clanId = it->second->getClanId();
  repository.save(playerName, it->second->toPlayerData());

  if (clanId != 0) {
    sendToClan(clanId, makeClanMessage(playerName + " salio de Argentum"),
               playerId);
  }

  colisionables.erase(
      std::remove(colisionables.begin(), colisionables.end(), it->second.get()),
      colisionables.end());

  messagesToSend.push_back(PlayerRemovedEventDTO{playerId});

  playerIdByName.erase(playerName);
  cheatsByPlayer.erase(playerId);
  players.erase(it);
}

void Game::equipItem(uint32_t playerId, uint8_t inventorySlot) {
  inventoryManager.equipItem(playerId, inventorySlot);
}

void Game::unequipSlot(uint32_t playerId, uint8_t equipSlot) {
  inventoryManager.unequipSlot(playerId, static_cast<EquipSlot>(equipSlot));
}

void Game::dropItem(uint32_t playerId, uint8_t inventorySlot) {
  inventoryManager.dropItem(playerId, inventorySlot);
}

void Game::takeItem(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end() || it->second->isDead())
    return;
  inventoryManager.takeItem(playerId);
}

void Game::sendPrivateMessage(uint32_t connectionId,
                              const std::string &targetName,
                              const std::string &message) {

  auto senderIt = players.find(connectionId);
  if (senderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(targetName);
  if (!targetPlayerId.has_value()) {
    sendToPlayer(senderIt->second->getId(),
                 makeSystemErrorMessage(
                     "Ese jugador no existe o no esta online"));
    return;
  }

  const std::string &senderName = senderIt->second->getName();
  sendToPlayer(targetPlayerId.value(),
               PrivateMessageEventDTO{senderName, targetName, message});
  sendToPlayer(senderIt->second->getId(),
               PrivateMessageEventDTO{senderName, targetName, message});
}

void Game::applyCheat(uint32_t connectionId, CheatType cheat) {
  uint32_t playerId = connectionId;
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    sendErrorMessageToConnection(connectionId, "No se pudo aplicar el cheat");
    return;
  }

  Character &player = *playerIt->second;
  PlayerCheats &cheats = cheatsByPlayer[playerId];

  switch (cheat) {
  case CheatType::Die:
    if (player.isDead()) {
      sendToPlayer(playerId, makeSystemErrorMessage("Ya estas muerto"));
      return;
    }
    killPlayer(player);
    messagesToSend.push_back(player.toPlayerInfoEvent());
    sendSystemMessageToPlayer(playerId, "Has muerto");
    return;

  case CheatType::InfiniteHealth:
    cheats.infiniteHealth = true;
    sendSystemMessageToPlayer(playerId, "Vida infinita activada");
    return;

  case CheatType::NormalHealth:
    cheats.infiniteHealth = false;
    sendSystemMessageToPlayer(playerId, "Vida infinita desactivada");
    return;

  case CheatType::InfiniteMana:
    cheats.infiniteMana = true;
    sendSystemMessageToPlayer(playerId, "Mana infinito activado");
    return;

  case CheatType::NormalMana:
    cheats.infiniteMana = false;
    sendSystemMessageToPlayer(playerId, "Mana infinito desactivado");
    return;

  case CheatType::SuperSpeed:
    cheats.superSpeed = true;
    sendSystemMessageToPlayer(playerId, "Supervelocidad activada");
    return;

  case CheatType::NormalSpeed:
    cheats.superSpeed = false;
    sendSystemMessageToPlayer(playerId, "Supervelocidad desactivada");
    return;
  }
}

void Game::createClan(uint32_t playerId, const std::string &clanName) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    return;
  }

  const std::string &playerName = playerIt->second->getName();
  ClanCreateResult result = clanManager.createClan(clanName, playerName);
  if (result == ClanCreateResult::Success) {
    uint32_t clanId = clanManager.getClanIdByName(clanName);
    playerIt->second->joinClan(clanId);
    repository.save(playerIt->second->getName(),
                    playerIt->second->toPlayerData());
    sendToPlayer(playerId, makeClanMessage("Fundaste el clan " + clanName));
    return;
  }

  std::string message = "No se pudo fundar el clan";
  if (result == ClanCreateResult::InvalidName) {
    message = "El nombre del clan no puede estar vacio";
  } else if (result == ClanCreateResult::NameAlreadyExists) {
    message = "Ya existe un clan con ese nombre";
  } else if (result == ClanCreateResult::PlayerAlreadyInClan) {
    message = "Ya perteneces a un clan";
  }
  sendToPlayer(playerId, makeClanErrorMessage(message));
}

void Game::requestJoinClan(uint32_t playerId, const std::string &clanName) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    return;
  }

  ClanJoinRequestResult result =
      clanManager.requestJoinClan(clanName, playerIt->second->getName());
  if (result == ClanJoinRequestResult::Success) {
    sendToPlayer(
        playerId, makeClanMessage("Solicitud enviada al clan " + clanName));

    uint32_t clanId = clanManager.getClanIdByName(clanName);
    const Clan *clan = clanManager.getClan(clanId);
    if (clan != nullptr) {
      auto founderId = findPlayerIdByName(clan->founderName);
      if (founderId.has_value()) {
        sendToPlayer(founderId.value(),
                     makeClanMessage(playerIt->second->getName() +
                                     " solicito unirse al clan " + clanName));
      }
    }
    return;
  }

  std::string message = "No se pudo enviar la solicitud al clan";
  if (result == ClanJoinRequestResult::ClanNotFound) {
    message = "No existe un clan con ese nombre";
  } else if (result == ClanJoinRequestResult::PlayerAlreadyInClan) {
    message = "Ya perteneces a un clan";
  } else if (result == ClanJoinRequestResult::ClanFull) {
    message = "El clan esta lleno";
  } else if (result == ClanJoinRequestResult::PlayerBanned) {
    message = "No puedes unirte a ese clan";
  } else if (result == ClanJoinRequestResult::AlreadyRequested) {
    message = "Ya enviaste una solicitud a ese clan";
  }
  sendToPlayer(playerId, makeClanErrorMessage(message));
}

void Game::acceptClanRequest(uint32_t founderId,
                             const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId, makeClanErrorMessage("Jugador no encontrado"));
    return;
  }

  const std::string &founderName = founderIt->second->getName();
  uint32_t clanId = clanManager.getClanId(founderName);
  const Clan *clan = clanManager.getClan(clanId);
  std::string clanName = clan != nullptr ? clan->name : "";

  ClanAcceptResult result =
      clanManager.acceptJoinRequest(founderName, playerName);
  if (result == ClanAcceptResult::Success) {
    if (targetPlayerId.has_value()) {
      auto playerIt = players.find(targetPlayerId.value());
      if (playerIt != players.end()) {
        playerIt->second->joinClan(clanId);
        repository.save(playerIt->second->getName(),
                        playerIt->second->toPlayerData());
      }

      sendToPlayer(targetPlayerId.value(),
                   makeClanMessage("Bienvenido al clan " + clanName));
      sendToClan(clanId,
                 makeClanMessage("El jugador " + playerName +
                                 " se unio al clan"),
                 targetPlayerId.value());
    } else {
      PlayerData data = repository.load(playerName);
      data.clanId = clanId;
      repository.save(playerName, data);

      sendToClan(clanId, makeClanMessage("El jugador " + playerName +
                                         " se unio al clan"));
    }
    return;
  }

  std::string message = "No se pudo aceptar la solicitud";
  if (result == ClanAcceptResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanAcceptResult::NotFounder) {
    message = "Solo el fundador puede aceptar solicitudes";
  } else if (result == ClanAcceptResult::RequestNotFound) {
    message = "No existe una solicitud pendiente de ese jugador";
  } else if (result == ClanAcceptResult::ClanFull) {
    message = "El clan esta lleno";
  } else if (result == ClanAcceptResult::PlayerAlreadyInClan) {
    message = "Ese jugador ya pertenece a un clan";
  }
  sendToPlayer(founderId, makeClanErrorMessage(message));
}

void Game::rejectClanRequest(uint32_t founderId,
                             const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId, makeClanErrorMessage("Jugador no encontrado"));
    return;
  }

  ClanRejectResult result =
      clanManager.rejectJoinRequest(founderIt->second->getName(), playerName);
  if (result == ClanRejectResult::Success) {
    sendToPlayer(founderId, makeClanMessage("Solicitud rechazada"));
    if (targetPlayerId.has_value()) {
      sendToPlayer(targetPlayerId.value(),
                   makeClanMessage("Tu solicitud fue rechazada"));
    }
    return;
  }

  std::string message = "No se pudo rechazar la solicitud";
  if (result == ClanRejectResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanRejectResult::NotFounder) {
    message = "Solo el fundador puede rechazar solicitudes";
  } else if (result == ClanRejectResult::RequestNotFound) {
    message = "No existe una solicitud pendiente de ese jugador";
  }
  sendToPlayer(founderId, makeClanErrorMessage(message));
}

void Game::banClanPlayer(uint32_t founderId, const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId, makeClanErrorMessage("Jugador no encontrado"));
    return;
  }

  const std::string &founderName = founderIt->second->getName();
  uint32_t clanId = clanManager.getClanId(founderName);
  ClanBanResult result = clanManager.banPlayer(founderName, playerName);
  if (result == ClanBanResult::Success) {
    if (targetPlayerId.has_value()) {
      auto playerIt = players.find(targetPlayerId.value());
      playerIt->second->leaveClan();
      repository.save(playerIt->second->getName(),
                      playerIt->second->toPlayerData());

      sendToPlayer(targetPlayerId.value(),
                   makeClanMessage("Fuiste baneado del clan"));
      sendToClan(clanId,
                 makeClanMessage("El jugador " + playerName +
                                 " fue baneado del clan"),
                 targetPlayerId.value());
    } else {
      PlayerData data = repository.load(playerName);
      data.clanId = 0;
      repository.save(playerName, data);
      sendToClan(clanId, makeClanMessage("El jugador " + playerName +
                                         " fue baneado del clan"));
    }
    return;
  }

  std::string message = "No se pudo banear al jugador";
  if (result == ClanBanResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanBanResult::NotFounder) {
    message = "Solo el fundador puede banear jugadores";
  } else if (result == ClanBanResult::CannotBanFounder) {
    message = "No puedes banearte a vos mismo";
  } else if (result == ClanBanResult::AlreadyBanned) {
    message = "Ese jugador ya esta baneado";
  }
  sendToPlayer(founderId, makeClanErrorMessage(message));
}

void Game::kickClanMember(uint32_t founderId, const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId, makeClanErrorMessage("Jugador no encontrado"));
    return;
  }

  const std::string &founderName = founderIt->second->getName();
  uint32_t clanId = clanManager.getClanId(founderName);
  ClanKickResult result = clanManager.kickMember(founderName, playerName);
  if (result == ClanKickResult::Success) {
    if (targetPlayerId.has_value()) {
      auto playerIt = players.find(targetPlayerId.value());
      playerIt->second->leaveClan();
      repository.save(playerIt->second->getName(),
                      playerIt->second->toPlayerData());

      sendToPlayer(targetPlayerId.value(),
                   makeClanMessage("Fuiste expulsado del clan"));
      sendToClan(clanId,
                 makeClanMessage("El jugador " + playerName +
                                 " fue expulsado del clan"),
                 targetPlayerId.value());
    } else {
      PlayerData data = repository.load(playerName);
      data.clanId = 0;
      repository.save(playerName, data);
      sendToClan(clanId, makeClanMessage("El jugador " + playerName +
                                         " fue expulsado del clan"));
    }
    return;
  }

  std::string message = "No se pudo expulsar al jugador";
  if (result == ClanKickResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanKickResult::NotFounder) {
    message = "Solo el fundador puede expulsar jugadores";
  } else if (result == ClanKickResult::CannotKickFounder) {
    message = "No se puede expulsar al fundador";
  } else if (result == ClanKickResult::TargetNotInClan) {
    message = "Ese jugador no pertenece a tu clan";
  }
  sendToPlayer(founderId, makeClanErrorMessage(message));
}

void Game::leaveClan(uint32_t playerId) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    return;
  }

  const std::string &playerName = playerIt->second->getName();
  uint32_t clanId = clanManager.getClanId(playerName);
  ClanLeaveResult result = clanManager.leaveClan(playerName);
  if (result == ClanLeaveResult::Success) {
    playerIt->second->leaveClan();
    repository.save(playerIt->second->getName(),
                    playerIt->second->toPlayerData());

    sendToPlayer(playerId, makeClanMessage("Saliste del clan"));
    sendToClan(clanId,
               makeClanMessage("El jugador " + playerName + " salio del clan"),
               playerId);
    return;
  }

  std::string message = "No se pudo salir del clan";
  if (result == ClanLeaveResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanLeaveResult::FounderCannotLeave) {
    message = "El fundador no puede salir del clan";
  }
  sendToPlayer(playerId, makeClanErrorMessage(message));
}

void Game::reviewClan(uint32_t playerId) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    return;
  }

  const std::string &playerName = playerIt->second->getName();
  if (!clanManager.hasClan(playerName)) {
    sendToPlayer(playerId,
                 makeClanErrorMessage("No perteneces a un clan"));
    return;
  }

  uint32_t clanId = clanManager.getClanId(playerName);
  const Clan *clan = clanManager.getClan(clanId);
  if (clan == nullptr) {
    sendToPlayer(playerId,
                 makeClanErrorMessage("No perteneces a un clan"));
    return;
  }

  std::ostringstream members;
  members << "Miembros de " << clan->name << ": ";
  std::vector<std::string> memberNames = clanManager.getMembers(clanId);
  for (size_t i = 0; i < memberNames.size(); ++i) {
    if (i > 0) {
      members << ", ";
    }
    members << memberNames[i];
  }
  sendToPlayer(playerId, makeClanMessage(members.str()));

  if (!clanManager.isFounder(playerName)) {
    return;
  }

  std::vector<std::string> pendingNames =
      clanManager.getPendingRequests(clanId);
  std::ostringstream pending;
  pending << "Pedidos pendientes: ";
  if (pendingNames.empty()) {
    pending << "ninguno";
  } else {
    for (size_t i = 0; i < pendingNames.size(); ++i) {
      if (i > 0) {
        pending << ", ";
      }
      pending << pendingNames[i];
    }
  }
  sendToPlayer(playerId, makeClanMessage(pending.str()));
}

bool Game::thereIsACollidableEntityAt(Position position) {
  int center = maxSize / 2;
  int cellX = (position.row - center) * gridSize;
  int cellY = (position.column - center) * gridSize;

  if (collidableCells.count({position.row, position.column, 0}))
    return true;

  for (auto &col : colisionables) {
    if (!(cellX + gridSize <= col->getX() ||
          cellX >= col->getX() + col->getAncho() ||
          cellY + gridSize <= col->getY() ||
          cellY >= col->getY() + col->getAlto()))
      return true;
  }

  return false;
}

void Game::appearNPC(std::unique_ptr<NPC> &&npc) {
  int center = maxSize / 2;
  int px = (npc->getPosition().row - center) * gridSize;
  int py = (npc->getPosition().column - center) * gridSize;
  npc->setPixelPosition(px, py);

  uint32_t id = nextNPCId++;
  npc->setId(id);

  colisionables.push_back(npc.get());

  messagesToSend.push_back(
      NPCAppearedEventDTO{id, static_cast<uint8_t>(npc->getType()),
                          static_cast<int16_t>(px), static_cast<int16_t>(py)});

  npcs.push_back(std::move(npc));
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {

    if (isResurrecting(playerID))
      continue;

    if (!info->isMoving())
      continue;

    auto [targetX, targetY] = info->getTargetPosition(movementSpeedFor(playerID));

    int origX = info->getX();
    int origY = info->getY();

    info->move(targetX, targetY);

    if (checkIfItCollides(info.get())) {
      info->move(origX, origY);
      continue;
    }

    messagesToSend.push_back(info->toPlayerMoved());
  }
}

void Game::sendMessages() {
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
}

void Game::saveAllPlayers() {
  for (auto &[id, player] : players) {
    repository.save(player->getName(), player->toPlayerData());
  }
}

void Game::sendGlobalChatMessage(uint32_t playerId,
                                 const std::string &message) {
  messagesToSend.push_back(
      GlobalChatMessageEventDTO{getPlayerName(playerId), message});
}

void Game::attack(uint32_t playerId, int16_t x, int16_t y) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  auto attacker = it->second.get();
  if (!attacker->assertAttackDistance(x, y)) {
    return;
  }
  auto targetNPC = findNPCByCoordinates(x, y);
  if (targetNPC != nullptr) {
    playerAttackNPC(*attacker, *targetNPC);
    return;
  }

  auto targetPlayer = findPlayerByCoordinates(x, y);
  if (targetPlayer != nullptr) {
    playerAttackPlayer(*attacker, *targetPlayer);
    return;
  }
}

void Game::appearNPCs() {
  for (auto &biome : biomes) {
    biome->NPCgenerationStrategy(*this);
  }
}

std::optional<uint32_t>
Game::findPlayerIdByName(const std::string &name) const {
  auto it = playerIdByName.find(name);
  if (it == playerIdByName.end())
    return std::nullopt;
  return it->second;
}

std::string Game::getPlayerName(uint32_t playerId) const {
  auto it = players.find(playerId);
  if (it == players.end())
    return "Player " + std::to_string(playerId);
  return it->second->getName();
}

void Game::sendToPlayer(uint32_t playerId, const ServerEventDTO &event) {
  senderQueueMonitor.sendToClient(playerId, event);
}

void Game::sendToPlayers(const std::vector<uint32_t> &playerIds,
                         const ServerEventDTO &event) {
  for (uint32_t playerId : playerIds) {
    sendToPlayer(playerId, event);
  }
}

void Game::sendToClan(uint32_t clanId, const ServerEventDTO &event,
                      std::optional<uint32_t> exceptPlayerId) {
  std::vector<std::string> members = clanManager.getMembers(clanId);
  for (const std::string &playerName : members) {
    auto playerId = findPlayerIdByName(playerName);
    if (!playerId.has_value()) {
      continue;
    }
    if (exceptPlayerId.has_value() &&
        playerId.value() == exceptPlayerId.value()) {
      continue;
    }
    sendToPlayer(playerId.value(), event);
  }
}

void Game::sendSystemMessage(uint32_t playerId, const std::string &msg) {
  senderQueueMonitor.sendToClient(playerId, makeSystemMessage(msg));
}

void Game::sendSystemMessageToPlayer(uint32_t playerId,
                                     const std::string &message) {
  sendToPlayer(playerId, makeSystemMessage(message));
}

void Game::sendErrorMessageToConnection(uint32_t connectionId,
                                        const std::string &message) {
  senderQueueMonitor.sendToClient(connectionId, makeSystemErrorMessage(message));
}

uint32_t Game::movementSpeedFor(uint32_t playerId) const {
  auto it = cheatsByPlayer.find(playerId);
  if (it != cheatsByPlayer.end() && it->second.superSpeed) {
    return 4;
  }
  return 2;
}

bool Game::hasInfiniteHealth(uint32_t playerId) const {
  auto it = cheatsByPlayer.find(playerId);
  return it != cheatsByPlayer.end() && it->second.infiniteHealth;
}

bool Game::hasInfiniteMana(uint32_t playerId) const {
  auto it = cheatsByPlayer.find(playerId);
  return it != cheatsByPlayer.end() && it->second.infiniteMana;
}

bool Game::checkIfItCollides(Colisionable *entity) {
  for (auto &col : colisionables) {
    if (col == entity)
      continue;
    if (col->colisionaCon(entity->getX(), entity->getY(), entity->getAncho(),
                          entity->getAlto()))
      return true;
  }

  int start_i = floorDiv(entity->getX(), gridSize) + maxSize / 2;
  int end_i =
      floorDiv(entity->getX() + entity->getAncho(), gridSize) + maxSize / 2;
  int start_j = floorDiv(entity->getY(), gridSize) + maxSize / 2;
  int end_j =
      floorDiv(entity->getY() + entity->getAlto(), gridSize) + maxSize / 2;
  for (int i = start_i; i <= end_i; i++) {
    for (int j = start_j; j <= end_j; j++) {
      if (collidableCells.count({i, j, 0}))
        return true;
    }
  }

  return false;
}

void Game::createCityEntities() {
  for (auto &city : cities) {
    city.createEntities(*this);

    for (auto *entity : city.getEntities()) {
      int center = maxSize / 2;
      int px = (entity->getPosition().row - center) * gridSize;
      int py = (entity->getPosition().column - center) * gridSize;
      entity->setPixelPosition(px, py);

      uint32_t id = nextCityEntityId++;
      entity->setId(id);
      colisionables.push_back(entity);

      messagesToSend.push_back(CityEntityAppearedEventDTO{
          id, static_cast<uint8_t>(entity->getCityEntityType()),
          static_cast<int16_t>(px), static_cast<int16_t>(py),
          entity->getDirection()});
    }
  }
}




void Game::tryAttack(NPC& npc, Character& target) {

  if (!npc.collidesWith(target) || !npc.reachesAttackCounter())
    return;

  if (target.tryParry()) {
    senderQueueMonitor.sendToClient(
        target.getId(),
        makeCombatMessage(npc.getName() + " trato de atacarte pero lo esquivaste"));
    return;
  }

  target.takeDamage(npc.getDamage());

  if (target.getHp() <= 0) {
    killPlayer(target);
    return;
  }

  messagesToSend.push_back(
      AttackReceivedEventDTO{EntityType::Player, target.getId(),
                             EffectType::NormalAttack});
  messagesToSend.push_back(target.toPlayerInfoEvent());
}



void Game::makeNPCsfollowPlayers()
{
  for (auto& npc : npcs) {
    Character* target = nullptr;

    for (auto &[_, player] : players) {
      if (player->isInCity(cities, gridSize, maxSize) || player->isDead())
        continue;

      int dx = npc->getX() - player->getX();
      int dy = npc->getY() - player->getY();
      if (abs(dx) <= npc->getRange() && abs(dy) <= npc->getRange()) {
        target = player.get();
        break;
      }
    }

    if (target) {
      int oldX = npc->getX();
      int oldY = npc->getY();

      if (npc->updatePosition(*target)) {
        if (checkIfItCollides(npc.get())) {

          tryAttack(*npc, *target);

          npc->setPixelPosition(oldX, oldY);
          npc->stop();
          messagesToSend.push_back(NPCStoppedEventDTO{npc->getId()});
        } else {
          messagesToSend.push_back(NPCMovedEventDTO{
              npc->getId(), static_cast<int16_t>(npc->getX()),
              static_cast<int16_t>(npc->getY()), npc->getDirection()});
        }
      }
    } else {
      if (npc->getIsMoving()) {
        npc->stop();
        messagesToSend.push_back(NPCStoppedEventDTO{npc->getId()});
      }
    }
  }
}


void Game::makeCitiesEntitiesFollowPlayers() {
  for (auto &city : cities) {
    for (auto *cityEntity : city.getEntities()) {
      Character *target = nullptr;

      for (auto &[_, player] : players) {
        if (!city.contains(player->getX(), player->getY(), gridSize, maxSize) || player->isDead())
          continue;

        int dx = cityEntity->getX() - player->getX();
        int dy = cityEntity->getY() - player->getY();
        if (abs(dx) <= cityEntity->getRange() && abs(dy) <= cityEntity->getRange()) {
            target = player.get();
            break;
        }
      }

            if (target) {
                int oldX = cityEntity->getX();
                int oldY = cityEntity->getY();

                if (cityEntity->updatePosition(*target)) {
                    if (checkIfItCollides(cityEntity)) {
                        cityEntity->setPixelPosition(oldX, oldY);
                        cityEntity->stop();
                        messagesToSend.push_back(CityEntityStoppedEventDTO{cityEntity->getId()});
                    } else {
                        messagesToSend.push_back(
                            CityEntityMovedEventDTO{cityEntity->getId(),
                                                    static_cast<int16_t>(cityEntity->getX()),
                                                    static_cast<int16_t>(cityEntity->getY()),
                                                    cityEntity->getDirection()});
                    }
                } else {
                  if (cityEntity->getIsMoving()) {
                    cityEntity->stop();
                    messagesToSend.push_back(CityEntityStoppedEventDTO{cityEntity->getId()});
                  }
                }
            } else {
                if (cityEntity->getIsMoving()) {
                    cityEntity->stop();
                    messagesToSend.push_back(CityEntityStoppedEventDTO{cityEntity->getId()});
                }
            }
        }
      }
    }

    
void Game::playerAttackPlayer(Character &attacker, Character &target) {

  if (!validAttack(attacker, target)) {
    return;
  }
  uint32_t damage = calculateDamage(attacker);
  bool critico = (damage != attacker.getDamage());

  if (!critico && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        makeCombatMessage("Atacaste a " + target.getName() +
                          " pero el lo esquivo"));
    senderQueueMonitor.sendToClient(
        target.getId(),
        makeCombatMessage(attacker.getName() +
                          " trato de atacarte pero lo esquivaste"));
    return;
  }
  if (hasInfiniteHealth(target.getId())) {
    damage = 0;
  } else {
    damage = target.takeDamage(damage);
  }

  messagesToSend.push_back(
      AttackReceivedEventDTO{EntityType::Player, target.getId(),
                             EffectType::NormalAttack});

  uint32_t xp = Formulas::calcularExperiencia(damage, attacker.getLevel(),
                                              target.getLevel());
  attacker.gainExperience(xp);

  if (target.getHp() <= 0) {
    uint32_t oro = target.dropGoldOnDeath();
    attacker.addGold(oro);
    uint32_t xpMuerte = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), attacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    attacker.gainExperience(xpMuerte);
    killPlayer(target);
  } else {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        makeCombatMessage("Atacaste a " + target.getName() +
                          " y le hiciste " + std::to_string(damage) +
                          " de daño!"));
    senderQueueMonitor.sendToClient(
        target.getId(),
        makeCombatMessage("Recibiste un ataque de " + attacker.getName() +
                          " y te hicieron " + std::to_string(damage) +
                          " de daño!"));
  }
  messagesToSend.push_back(attacker.toPlayerInfoEvent());
  messagesToSend.push_back(target.toPlayerInfoEvent());
}

void Game::playerAttackNPC(Character &attacker, NPC &target) {

  if (!validAttackToNpc(attacker))
    return;

  uint32_t damage = calculateDamage(attacker);

  bool critico = (damage != attacker.getDamage());

  if (!critico && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        makeCombatMessage("Atacaste a " + target.getName() + " pero lo esquivo"));
    return;
  }

  target.takeDamage(damage);

  messagesToSend.push_back(
      AttackReceivedEventDTO{EntityType::Npc, target.getId(),
                             EffectType::NormalAttack});

  uint32_t xp = Formulas::calcularExperiencia(damage, attacker.getLevel(),
                                              target.getLevel());
  attacker.gainExperience(xp);

  if (target.getHP() <= 0) {

    ObjectDropped objectDropped = target.getDroppedObject();

    switch (objectDropped.type) {
      case ObjectDroppedType::Gold:
        attacker.addGold(objectDropped.value);
        break;

      case ObjectDroppedType::Item:
        inventoryManager.addGroundItem(
            static_cast<uint8_t>(objectDropped.value), target.getX(),
            target.getY());
        break;

      default:
        break;
    }

    uint32_t xpMuerte = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), attacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    attacker.gainExperience(xpMuerte);

    messagesToSend.push_back(NpcDefeatedEventDTO{target.getId()});

    colisionables.erase(
        std::remove(colisionables.begin(), colisionables.end(), &target),
        colisionables.end());

    auto npcIt = std::find_if(
        npcs.begin(), npcs.end(),
        [&target](const auto &npc) { return npc.get() == &target; });
    if (npcIt != npcs.end()) {
      npcs.erase(npcIt);
    }

  } else {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        makeCombatMessage("Atacaste a un " + target.getName() +
                          " y le hiciste " + std::to_string(damage) +
                          " de daño!"));
  }
  messagesToSend.push_back(attacker.toPlayerInfoEvent());

}

uint32_t Game::calculateDamage(Character &attacker) {
  uint32_t damage = attacker.getDamage();
  if (Formulas::calcularCritico(std::rand())) {
    return damage * 2;
  }
  return damage;
}

bool Game::validAttack(Character &attacker, Character &target) {
  if (attacker.isNewbie() || target.isNewbie()) {
    return false;
  }

  if (attacker.getId() == target.getId())
    return false;

  if (abs(static_cast<int>(attacker.getLevel()) -
          static_cast<int>(target.getLevel())) > 10) {
    return false;
  }
  for (const auto &city : cities) {
    if (city.contains(attacker.getX(), attacker.getY(), gridSize, maxSize) ||
        city.contains(target.getX(), target.getY(), gridSize, maxSize)) {
      return false;
    }
  }
  if (attacker.isDead() || target.isDead()) {
    return false;
  }
  return true;
}

bool Game::validAttackToNpc(Character &attacker) {
  if (attacker.isDead())
    return false;
  for (const auto &city : cities) {
    if (city.contains(attacker.getX(), attacker.getY(), gridSize, maxSize)) {
      return false;
    }
  }
  return true;
}

Character *Game::findPlayerByCoordinates(int16_t x, int16_t y) {
  for (auto &[pid, player] : players) {
    if (player->colisionaCon(x, y, 16, 16)) {
      return player.get();
    }
  }
  return nullptr;
}

NPC *Game::findNPCByCoordinates(int16_t x, int16_t y) {
  for (auto &npc : npcs) {
    if (npc->colisionaCon(x, y, 16, 16)) {
      return npc.get();
    }
  }
  return nullptr;
}

void Game::killPlayer(Character &dyingPlayer) {
  dyingPlayer.dropGoldOnDeath();
  auto items = dyingPlayer.die();
  int16_t x = dyingPlayer.getX();
  int16_t y = dyingPlayer.getY();
  for (auto itemId : items) {
    inventoryManager.addGroundItem(itemId, x, y);
  }
  messagesToSend.push_back(PlayerDieEventDTO{dyingPlayer.getId()});
}


void Game::sendInventoryUpdate(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  messagesToSend.push_back(InventoryUpdateEventDTO{
      playerId, it->second->getInventoryItems(),
      it->second->getEquippedWeapon(), it->second->getEquippedArmor(),
      it->second->getEquippedHelmet(), it->second->getEquippedShield()});
}

void Game::sendPlayerInfoUpdate(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  messagesToSend.push_back(it->second->toPlayerInfoEvent());
}

void Game::sendPlayerMoved(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  messagesToSend.push_back(it->second->toPlayerMoved());
}

bool Game::isNearEntity(CityEntity &entity, const Character &character) {
  int dx = entity.getX() - character.getX();
  int dy = entity.getY() - character.getY();
  return abs(dx) <= entity.getRange() && abs(dy) <= entity.getRange();
}

CityEntity *Game::findNearestEntity(uint32_t playerId, CityEntityType type) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end())
    return nullptr;
  auto &character = playerIt->second;
  CityEntity *nearest = nullptr;
  int minDist = INT_MAX;

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      if (entity->getCityEntityType() != type)
        continue;
      int dx = entity->getX() - character->getX();
      int dy = entity->getY() - character->getY();
      int dist = dx * dx + dy * dy;
      if (dist < minDist) {
        minDist = dist;
        nearest = entity;
      }
    }
  }
  return nearest;
}

void Game::executeCityEntityCommand(uint32_t playerId, uint8_t type,
                                    int16_t arg) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end())
    return;
  auto &character = playerIt->second;

  if (character->isDead() && 
      static_cast<CityEntityCommandDTO::Type>(type) != CityEntityCommandDTO::RESUCITAR)
    return;

  switch (static_cast<CityEntityCommandDTO::Type>(type)) {

  case CityEntityCommandDTO::CURAR: {
    auto *priest = dynamic_cast<Priest *>(
        findNearestEntity(playerId, CityEntityType::Priest));
    if (!priest || !isNearEntity(*priest, *character)) {

      sendSystemMessage(playerId, "No estas cerca de un sacerdote.");
      
      return;
    }
    priest->heal(*this, *character);
    break;
  }

  case CityEntityCommandDTO::RESUCITAR: {
    if (!character->isDead()) {
      sendSystemMessage(playerId, "No estas muerto.");
      return;
    }
    if (isResurrecting(playerId)) {
      sendSystemMessage(playerId, "Ya estas siendo resucitado.");
      return;
    }
    auto *priest = dynamic_cast<Priest *>(
        findNearestEntity(playerId, CityEntityType::Priest));
    if (!priest) {
      sendSystemMessage(playerId, "No hay ningun sacerdote disponible para resucitarte.");
      return;
    }
    priest->resurrect(*this, *character);
    break;
  }

  case CityEntityCommandDTO::COMPRAR: {
    if (arg < 1 || arg >= 20) {
      sendSystemMessage(playerId, "ID de objeto invalido.");
      return;
    }
    uint8_t itemId = static_cast<uint8_t>(arg);
    auto *priest = dynamic_cast<Priest *>(
        findNearestEntity(playerId, CityEntityType::Priest));
    if (priest && isNearEntity(*priest, *character)) {
      priest->buyItem(*this, *character, itemId);
      break;
    }
    auto *trader = dynamic_cast<Trader *>(
        findNearestEntity(playerId, CityEntityType::Trader));
    if (trader && isNearEntity(*trader, *character)) {
      trader->buyItem(*this, *character, itemId);
      break;
    }
    sendSystemMessage(playerId, "No estas cerca de un sacerdote o comerciante.");
    break;
  }

  case CityEntityCommandDTO::VENDER: {
    if (arg < 1 || arg >= 20) {
      sendSystemMessage(playerId, "ID de objeto invalido.");
      return;
    }
    auto *trader = dynamic_cast<Trader *>(
        findNearestEntity(playerId, CityEntityType::Trader));
    if (!trader || !isNearEntity(*trader, *character)) {
      sendSystemMessage(playerId, "No estas cerca de un comerciante.");
      return;
    }
    trader->sellItem(*this, *character, static_cast<uint8_t>(arg));
    break;
  }

  case CityEntityCommandDTO::LISTAR: {
    auto *priest = dynamic_cast<Priest *>(
        findNearestEntity(playerId, CityEntityType::Priest));
    if (priest && isNearEntity(*priest, *character)) {
      priest->listItems(*this, *character);
      break;
    }
    auto *trader = dynamic_cast<Trader *>(
        findNearestEntity(playerId, CityEntityType::Trader));
    if (trader && isNearEntity(*trader, *character)) {
      trader->listItems(*this, *character);
      break;
    }
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (banker && isNearEntity(*banker, *character)) {
      banker->listItemsAvailables(*this, *character);
      break;
    }
    sendSystemMessage(playerId, "No estas cerca de un sacerdote, comerciante o banquero.");
    break;
  }

  case CityEntityCommandDTO::CONSULTAR_ORO: {
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (!banker || !isNearEntity(*banker, *character)) {
      sendSystemMessage(playerId, "No estas cerca de un comerciante.");
      return;
    }
    banker->showGoldAvailable(*this, *character);
    break;
  }

  case CityEntityCommandDTO::DEPOSITAR_ITEM: {
    if (arg < 1 || arg >= 20) {
      sendSystemMessage(playerId, "ID de objeto invalido.");
      return;
    }
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (!banker || !isNearEntity(*banker, *character)) {
      sendSystemMessage(playerId, "No estas cerca de un banquero.");
      return;
    }
    banker->saveItem(*this, *character, static_cast<uint8_t>(arg));
    break;
  }

  case CityEntityCommandDTO::RETIRAR_ITEM: {
    if (arg < 1 || arg >= 20) {
      sendSystemMessage(playerId, "ID de objeto invalido.");
      return;
    }
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (!banker) {
      sendSystemMessage(playerId, "No hay ningun banquero disponible.");
      return;
    }
    banker->takeItem(*this, *character, static_cast<uint8_t>(arg));
    break;
  }

  case CityEntityCommandDTO::DEPOSITAR_ORO: {
    if (arg <= 0) {
      sendSystemMessage(playerId, "Cantidad invalida.");
      return;
    }
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (!banker || !isNearEntity(*banker, *character)) {
      sendSystemMessage(playerId, "No estas cerca de un banquero.");
      return;
    }
    banker->saveGold(*this, *character, static_cast<uint16_t>(arg));
    break;
  }

  case CityEntityCommandDTO::RETIRAR_ORO: {
    if (arg <= 0) {
      sendSystemMessage(playerId, "Cantidad invalida.");
      return;
    }
    auto *banker = dynamic_cast<Banker *>(
        findNearestEntity(playerId, CityEntityType::Banker));
    if (!banker) {
      sendSystemMessage(playerId, "No hay ningun banquero disponible.");
      return;
    }
    banker->takeGold(*this, *character, static_cast<uint16_t>(arg));
    break;
  }
  }
}


void Game::addResurrectingPlayer(Character &character, int priestX,
                                 int priestY, int maxCounter) {
  resurrectingPlayers.push_back(
      {&character, priestX, priestY, 0, maxCounter});
}

void Game::updateResurrectingPlayers() {
  auto it = resurrectingPlayers.begin();
  while (it != resurrectingPlayers.end()) {
    it->counter++;
    if (it->counter >= it->maxCounter) {
      uint32_t pid = it->character->getId();

      it->character->move(it->priestX, it->priestY);
      it->character->resurrect();

      sendSystemMessage(pid, "Has sido resucitado.");

      senderQueueMonitor.sendToClient(
          pid,
          PlayerResurrectEventDTO{pid, static_cast<int16_t>(it->priestX),
                                  static_cast<int16_t>(it->priestY)});

      messagesToSend.push_back(it->character->toPlayerAppeared());

      sendPlayerInfoUpdate(pid);

      it = resurrectingPlayers.erase(it);
    } else {
      ++it;
    }
  }
}

bool Game::isResurrecting(uint32_t playerId) {
  for (auto &rp : resurrectingPlayers) {
    if (rp.character->getId() == playerId)
      return true;
  }
  return false;
}