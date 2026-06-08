#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>

#include "ConstantRateLoop.h"
#include "Formulas.h"
#include "Game.h"
#include "GlobalChatMessageEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "NPCAppearedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStopCommandDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include "command/CommandFactory.h"

static int floorDiv(int a, int b) { return (a >= 0) ? a / b : (a - b + 1) / b; }

Game::Game(Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository,
           ClanManager &clanManager)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor),
      repository(repository), clanManager(clanManager) {}

void Game::run() {
  MapLoader mapLoader("map.toml");
  maxSize = mapLoader.GetMaxSize();
  gridSize = mapLoader.GetGridSize();
  commonGroundTextureId = mapLoader.GetCommonGroundTextureId();
  textureOrigins = mapLoader.GetTextureOrigins();
  collidableCells = mapLoader.GetCollidableCells();

  biomes = std::move(mapLoader.GetBiomes());
  cities = mapLoader.GetCities();

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
    movePlayers();
    appearNPCs();
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
    senderQueueMonitor.sendToClient(connectionId, RegisterPlayerEventDTO{0, 1});
    return;
  }

  uint32_t newId = nextPlayerId++;
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
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;
  playerIdByName[name] = newId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId, 0});

  {
    std::vector<TextureOriginDTO> origins;
    origins.reserve(textureOrigins.size());
    for (const auto &o : textureOrigins) {
      origins.push_back(
          {static_cast<uint8_t>(o.priority), static_cast<uint8_t>(o.texture_id),
           static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId,
        TextureInfoEventDTO{
            static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
            static_cast<uint8_t>(commonGroundTextureId), std::move(origins)});
  }

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(info->toPlayerInfo(pid));
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

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

void Game::loginPlayer(const std::string &name, uint32_t connectionId) {

  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId, RegisterPlayerEventDTO{0, 1});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->getName() == name) {
      senderQueueMonitor.sendToClient(connectionId,
                                      RegisterPlayerEventDTO{0, 2});
      return;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = nextPlayerId++;

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
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;
  playerIdByName[name] = newId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId, 0});

  std::vector<TextureOriginDTO> origins;
  origins.reserve(textureOrigins.size());
  for (const auto &o : textureOrigins) {
    origins.push_back({static_cast<uint8_t>(o.priority),
                       static_cast<uint8_t>(o.texture_id),
                       static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
  }
  senderQueueMonitor.sendToClient(
      connectionId,
      TextureInfoEventDTO{
          static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
          static_cast<uint8_t>(commonGroundTextureId), std::move(origins)});

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(info->toPlayerInfo(pid));
  }
  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

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

  uint32_t clanId = players[newId]->getClanId();
  if (clanId != 0) {
    sendToClan(clanId, ChatMessageEventDTO{"Clan", name + " entro a Argentum"},
               newId);
  }
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end()) {
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
    sendToClan(clanId,
               ChatMessageEventDTO{"Clan", playerName + " salio de Argentum"},
               playerId);
  }

  colisionables.erase(
      std::remove(colisionables.begin(), colisionables.end(), it->second.get()),
      colisionables.end());

  messagesToSend.push_back(PlayerRemovedEventDTO{playerId});

  auto connIt = playerToConnection.find(playerId);
  if (connIt != playerToConnection.end()) {
    connectionToPlayer.erase(connIt->second);
    playerToConnection.erase(connIt);
  }

  playerIdByName.erase(playerName);
  players.erase(it);
}

void Game::exitPlayerByConnection(uint32_t connectionId) {
  auto it = connectionToPlayer.find(connectionId);
  if (it == connectionToPlayer.end()) {
    return;
  }
  exitPlayer(it->second);
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

void Game::takeItem(uint32_t playerId) { inventoryManager.takeItem(playerId); }

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
    sendToPlayer(playerId,
                 ChatMessageEventDTO{"Clan", "Fundaste el clan " + clanName});
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
  sendToPlayer(playerId, ChatMessageEventDTO{"Clan", message});
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
        playerId,
        ChatMessageEventDTO{"Clan", "Solicitud enviada al clan " + clanName});

    uint32_t clanId = clanManager.getClanIdByName(clanName);
    const Clan *clan = clanManager.getClan(clanId);
    if (clan != nullptr) {
      auto founderId = findPlayerIdByName(clan->founderName);
      if (founderId.has_value()) {
        sendToPlayer(founderId.value(),
                     ChatMessageEventDTO{
                         "Clan", playerIt->second->getName() +
                                     " solicito unirse al clan " + clanName});
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
  sendToPlayer(playerId, ChatMessageEventDTO{"Clan", message});
}

void Game::acceptClanRequest(uint32_t founderId,
                             const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value()) {
    sendToPlayer(founderId,
                 ChatMessageEventDTO{"Clan", "Jugador no encontrado"});
    return;
  }

  const std::string &founderName = founderIt->second->getName();
  uint32_t clanId = clanManager.getClanId(founderName);
  const Clan *clan = clanManager.getClan(clanId);
  std::string clanName = clan != nullptr ? clan->name : "";

  ClanAcceptResult result =
      clanManager.acceptJoinRequest(founderName, playerName);
  if (result == ClanAcceptResult::Success) {
    auto playerIt = players.find(targetPlayerId.value());
    if (playerIt != players.end()) {
      playerIt->second->joinClan(clanId);
      repository.save(playerIt->second->getName(),
                      playerIt->second->toPlayerData());
    }

    sendToPlayer(targetPlayerId.value(),
                 ChatMessageEventDTO{"Clan", "Bienvenido al clan " + clanName});
    sendToClan(clanId,
               ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                               " se unio al clan"},
               targetPlayerId.value());
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
  sendToPlayer(founderId, ChatMessageEventDTO{"Clan", message});
}

void Game::rejectClanRequest(uint32_t founderId,
                             const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value()) {
    sendToPlayer(founderId,
                 ChatMessageEventDTO{"Clan", "Jugador no encontrado"});
    return;
  }

  ClanRejectResult result =
      clanManager.rejectJoinRequest(founderIt->second->getName(), playerName);
  if (result == ClanRejectResult::Success) {
    sendToPlayer(founderId, ChatMessageEventDTO{"Clan", "Solicitud rechazada"});
    sendToPlayer(targetPlayerId.value(),
                 ChatMessageEventDTO{"Clan", "Tu solicitud fue rechazada"});
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
  sendToPlayer(founderId, ChatMessageEventDTO{"Clan", message});
}

void Game::banClanPlayer(uint32_t founderId, const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId,
                 ChatMessageEventDTO{"Clan", "Jugador no encontrado"});
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
                   ChatMessageEventDTO{"Clan", "Fuiste baneado del clan"});
      sendToClan(clanId,
                 ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                                 " fue baneado del clan"},
                 targetPlayerId.value());
    } else {
      PlayerData data = repository.load(playerName);
      data.clanId = 0;
      repository.save(playerName, data);
      sendToClan(clanId,
                 ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                                 " fue baneado del clan"});
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
  sendToPlayer(founderId, ChatMessageEventDTO{"Clan", message});
}

void Game::kickClanMember(uint32_t founderId, const std::string &playerName) {
  auto founderIt = players.find(founderId);
  if (founderIt == players.end()) {
    return;
  }

  auto targetPlayerId = findPlayerIdByName(playerName);
  if (!targetPlayerId.has_value() && !repository.exists(playerName)) {
    sendToPlayer(founderId,
                 ChatMessageEventDTO{"Clan", "Jugador no encontrado"});
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
                   ChatMessageEventDTO{"Clan", "Fuiste expulsado del clan"});
      sendToClan(clanId,
                 ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                                 " fue expulsado del clan"},
                 targetPlayerId.value());
    } else {
      PlayerData data = repository.load(playerName);
      data.clanId = 0;
      repository.save(playerName, data);
      sendToClan(clanId,
                 ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                                 " fue expulsado del clan"});
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
  sendToPlayer(founderId, ChatMessageEventDTO{"Clan", message});
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

    sendToPlayer(playerId, ChatMessageEventDTO{"Clan", "Saliste del clan"});
    sendToClan(clanId,
               ChatMessageEventDTO{"Clan", "El jugador " + playerName +
                                               " salio del clan"},
               playerId);
    return;
  }

  std::string message = "No se pudo salir del clan";
  if (result == ClanLeaveResult::PlayerNotInClan) {
    message = "No perteneces a un clan";
  } else if (result == ClanLeaveResult::FounderCannotLeave) {
    message = "El fundador no puede salir del clan";
  }
  sendToPlayer(playerId, ChatMessageEventDTO{"Clan", message});
}

void Game::reviewClan(uint32_t playerId) {
  auto playerIt = players.find(playerId);
  if (playerIt == players.end()) {
    return;
  }

  const std::string &playerName = playerIt->second->getName();
  if (!clanManager.hasClan(playerName)) {
    sendToPlayer(playerId,
                 ChatMessageEventDTO{"Clan", "No perteneces a un clan"});
    return;
  }

  uint32_t clanId = clanManager.getClanId(playerName);
  const Clan *clan = clanManager.getClan(clanId);
  if (clan == nullptr) {
    sendToPlayer(playerId,
                 ChatMessageEventDTO{"Clan", "No perteneces a un clan"});
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
  sendToPlayer(playerId, ChatMessageEventDTO{"Clan", members.str()});

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
  sendToPlayer(playerId, ChatMessageEventDTO{"Clan", pending.str()});
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

  for (auto &npc : npcs) {
    if (!(cellX + gridSize <= npc->getX() ||
          cellX >= npc->getX() + npc->getAncho() ||
          cellY + gridSize <= npc->getY() ||
          cellY >= npc->getY() + npc->getAlto()))
      return true;
  }

  return false;
}

void Game::appearNPC(std::unique_ptr<NPC> &&npc) {
  int center = maxSize / 2;
  int px = (npc->getPosition().row - center) * gridSize;
  int py = (npc->getPosition().column - center) * gridSize;
  npc->setPixelPosition(px, py);

  uint16_t id = nextNPCId++;
  messagesToSend.push_back(
      NPCAppearedEventDTO{id, static_cast<uint8_t>(npc->getType()),
                          static_cast<int16_t>(px), static_cast<int16_t>(py)});

  std::cout << "NPC of type " << (int)npc->getType() << std::endl;
  npcs.push_back(std::move(npc));
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info->isMoving()) {
      continue;
    }

    auto [targetX, targetY] = info->getTargetPosition();

    bool blocked = false;
    for (auto &col : colisionables) {
      if (col == info.get())
        continue;
      if (col->colisionaCon(targetX, targetY, info->getAncho(),
                            info->getAlto())) {
        blocked = true;
        break;
      }
    }
    if (blocked)
      continue;

    {
      int start_i = floorDiv(targetX, gridSize) + maxSize / 2;
      int end_i =
          floorDiv(targetX + info->getAncho() - 1, gridSize) + maxSize / 2;
      int start_j = floorDiv(targetY, gridSize) + maxSize / 2;
      int end_j =
          floorDiv(targetY + info->getAlto() - 1, gridSize) + maxSize / 2;
      bool tileBlocked = false;
      for (int i = start_i; i <= end_i; i++) {
        for (int j = start_j; j <= end_j; j++) {
          if (collidableCells.count({i, j, 0})) {
            tileBlocked = true;
            break;
          }
        }
        if (tileBlocked)
          break;
      }
      if (tileBlocked)
        continue;
    }

    info->move(targetX, targetY);

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

void Game::atack(uint32_t playerId, int16_t x, int16_t y) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  auto atacker = it->second.get();
  if (!atacker->assertAtackDistance(x, y)) {
    return;
  }
  auto targetNPC = findNPCByCoordinates(x, y);
  if (targetNPC != nullptr) {
    playerAtackNPC(*atacker, *targetNPC);
    return;
  }

  auto targetPlayer = findPlayerByCoordinates(x, y);
  if (targetPlayer != nullptr) {
    playerAtackPlayer(*atacker, *targetPlayer);
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

std::optional<uint32_t>
Game::getConnectionIdForPlayer(uint32_t playerId) const {
  auto it = playerToConnection.find(playerId);
  if (it == playerToConnection.end()) {
    return std::nullopt;
  }
  return it->second;
}

void Game::sendToPlayer(uint32_t playerId, const ServerEventDTO &event) {
  auto connectionId = getConnectionIdForPlayer(playerId);
  if (!connectionId.has_value()) {
    return;
  }
  senderQueueMonitor.sendToClient(connectionId.value(), event);
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

void Game::playerAtackPlayer(Character &atacker, Character &target) {
  if (!validAtack(atacker, target)) {
    return;
  }
  uint32_t damage = calculateDamage(atacker);
  bool critico = (damage != atacker.getDamage());

  if (!critico && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{"Sistema", "Atacaste a " + target.getName() +
                                           " pero el lo esquivo"});
    senderQueueMonitor.sendToClient(
        playerToConnection[target.getId()],
        ChatMessageEventDTO{"Sistema",
                            atacker.getName() +
                                " trato de atacarte pero lo esquivaste"});
    return;
  }
  damage = target.takeDamage(damage);

  uint32_t xp = Formulas::calcularExperiencia(damage, atacker.getLevel(),
                                              target.getLevel());
  atacker.gainExperience(xp);

  if (target.getHp() == 0) {
    uint32_t oro = target.dropGoldOnDeath();
    atacker.addGold(oro);
    uint32_t xpMuerte = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), atacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    atacker.gainExperience(xpMuerte);
    // [TODO] volver fanstasma el target
  } else {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{
            "Sistema", "Atacaste a " + target.getName() + " y le hiciste " +
                           std::to_string(damage) + " de daño!"});
    senderQueueMonitor.sendToClient(
        playerToConnection[target.getId()],
        ChatMessageEventDTO{"Sistema",
                            "Recibiste un ataque de " + atacker.getName() +
                                " y te hicieron " + std::to_string(damage) +
                                " de daño!"});
  }
  messagesToSend.push_back(atacker.toPlayerInfoEvent());
  messagesToSend.push_back(target.toPlayerInfoEvent());
}

void Game::playerAtackNPC(Character &atacker, NPC &target) {
  uint32_t damage = calculateDamage(atacker);
  // [TODO] aplicar daño al npc
  /* [TODO] exp del ataque*/
  if (false) {
    // [TODO] muerte del npc
  } else {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{
            "Sistema", "Atacaste a un " + npcName(target) + " y le hiciste " +
                           std::to_string(damage) + " de daño!"});
  }
  messagesToSend.push_back(atacker.toPlayerInfoEvent());
}

uint32_t Game::calculateDamage(Character &atacker) {
  uint32_t damage = atacker.getDamage();
  if (Formulas::calcularCritico(std::rand())) {
    return damage * 2;
  }
  return damage;
}

bool Game::validAtack([[maybe_unused]] Character &atacker,
                      [[maybe_unused]] Character &target) {
  return !atacker.isNewbie() && !target.isNewbie() &&
         abs(static_cast<int>(atacker.getLevel()) -
             static_cast<int>(target.getLevel())) <= 10;
  // [TODO] validar si esta en ciudad
}

Character *Game::findPlayerByCoordinates(int16_t x, int16_t y) {
  for (auto &[pid, player] : players) {
    if (player->colisionaCon(x, y, player->getAncho(), player->getAlto())) {
      return player.get();
    }
  }
  return nullptr;
}

NPC *Game::findNPCByCoordinates(int16_t x, int16_t y) {
  for (auto &npc : npcs) {
    if (x >= npc->getX() && x < npc->getX() + npc->getAncho() &&
        y >= npc->getY() && y < npc->getY() + npc->getAlto()) {
      return npc.get();
    }
  }
  return nullptr;
}

const std::string Game::npcName(NPC &npc) {
  switch (npc.getType()) {
  case NPCType::ZombieT:
    return "Zombie";
  case NPCType::SpiderT:
    return "Spider";
  case NPCType::ElfT:
    return "Elf";
  case NPCType::SkeletonT:
    return "Skeleton";
  case NPCType::OrcT:
    return "Orc";
  case NPCType::GiantT:
    return "Giant";
  case NPCType::GolemT:
    return "Golem";
  default:
    return "NPC";
  }
}
