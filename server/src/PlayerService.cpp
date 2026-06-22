#include "PlayerService.h"
#include <algorithm>

PlayerService::PlayerService(
    SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository,
    ClanManager &clanManager, std::list<ServerEventDTO> &messagesToSend,
    std::vector<Colisionable *> &colisionables, std::list<City> &cities,
    std::list<std::unique_ptr<NPC>> &npcs, const int maxGridSize,
    const int gridSize, const int commonGroundTextureId,
    const std::list<TileOrigin> &textureOrigins,
    const std::vector<GroundItem> &groundItems)
    : senderQueueMonitor(senderQueueMonitor), repository(repository),
      clanManager(clanManager), messagesToSend(messagesToSend),
      colisionables(colisionables), maxGridSize(maxGridSize),
      gridSize(gridSize), commonGroundTextureId(commonGroundTextureId),
      cities(cities), npcs(npcs), textureOrigins(textureOrigins),
      groundItems(groundItems) {}

std::unordered_map<uint32_t, std::unique_ptr<Character>> &
PlayerService::getPlayers() {
  return players;
}

std::optional<Character *> PlayerService::getPlayer(uint32_t playerID) {
  auto it = players.find(playerID);
  if (it != players.end()) {
    return it->second.get();
  }
  return std::nullopt;
}

std::optional<Character *> PlayerService::getPlayer(const std::string &name) {
  auto it = playerIdByName.find(name);
  if (it == playerIdByName.end()) {
    return std::nullopt;
  }
  return getPlayer(it->second);
}

void PlayerService::registerPlayer(const std::string &name, const Race race,
                                   const PlayerClass playerClass,
                                   uint32_t connectionId) {
  if (repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId,
        RegisterPlayerEventDTO{0, RegisterStatus::PlayerAlreadyExists});
    return;
  }

  uint32_t newId = connectionId;
  auto [spawnX, spawnY] = playerSpawnPosition();

  auto player = std::make_unique<Character>(newId, name, race, playerClass,
                                            spawnX, spawnY, Direction::Down);
  player->addItem(17);
  player->addItem(1);
  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  playerIdByName[name] = newId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(
      connectionId, RegisterPlayerEventDTO{newId, RegisterStatus::Success});

  sendPlayerNewConnection(*players[newId]);
}

void PlayerService::validateLogin(const std::string &name,
                                  uint32_t connectionId) {
  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::PlayerNotFound});
  } else if (getPlayer(name).has_value()) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::AlreadyOnline});
  } else {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::Success});
  }
}

void PlayerService::loginPlayer(const std::string &name,
                                uint32_t connectionId) {
  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::PlayerNotFound});
    return;
  }

  if (getPlayer(name).has_value()) {
    senderQueueMonitor.sendToClient(
        connectionId, LoginResultEventDTO{0, LoginStatus::AlreadyOnline});
    return;
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

  senderQueueMonitor.sendToClient(
      connectionId, LoginResultEventDTO{newId, LoginStatus::Success});

  sendPlayerNewConnection(*players[newId]);

  if (players[newId]->isDead()) {
    messagesToSend.push_back(PlayerDieEventDTO{players[newId]->getId()});
  }

  uint32_t clanId = players[newId]->getClanId();
  if (clanId != 0) {
    auto clan = getClanOfPlayer(*(players[newId]));
    for (uint32_t memberId : clan) {
      if (memberId != newId) {
        senderQueueMonitor.sendToClient(
            memberId, ChatMessageEventDTO{ChatMessageCategory::Clan, "Clan",
                                          name + " entro a Argentum"});
      }
    }
  }
}

void PlayerService::exitPlayer(uint32_t playerId) {
  auto pOpt = getPlayer(playerId);
  if (!pOpt.has_value()) {
    return;
  }
  Character &player = *pOpt.value();
  const std::string playerName = player.getName();
  uint32_t clanId = player.getClanId();
  repository.save(playerName, player.toPlayerData());

  if (clanId != 0) {
    auto clan = getClanOfPlayer(player);
    for (uint32_t memberId : clan) {
      if (memberId != playerId) {
        senderQueueMonitor.sendToClient(
            memberId, ChatMessageEventDTO{ChatMessageCategory::Clan, "Clan",
                                          playerName + " salio de Argentum"});
      }
    }
  }

  colisionables.erase(
      std::remove(colisionables.begin(), colisionables.end(), &player),
      colisionables.end());

  messagesToSend.push_back(PlayerRemovedEventDTO{playerId});

  playerIdByName.erase(playerName);
  players.erase(playerId);
}

void PlayerService::sendPlayerNewConnection(Character &player) {
  {
    std::vector<TextureOriginDTO> origins;
    origins.reserve(textureOrigins.size());
    for (const auto &o : textureOrigins) {
      origins.push_back({static_cast<uint8_t>(o.priority),
                         static_cast<uint16_t>(o.texture_id),
                         static_cast<uint16_t>(o.x),
                         static_cast<uint16_t>(o.y)});
    }
    senderQueueMonitor.sendToClient(
        player.getId(),
        TextureInfoEventDTO{
            static_cast<uint16_t>(maxGridSize), static_cast<uint16_t>(gridSize),
            static_cast<uint16_t>(commonGroundTextureId), std::move(origins)});
  }

  {
    std::vector<PlayerInfoDTO> playerList;
    for (auto &[pid, info] : players) {
      playerList.push_back(info->toPlayerInfo(pid));
    }
    senderQueueMonitor.sendToClient(player.getId(),
                                    PlayerListEventDTO{std::move(playerList)});
  }

  for (auto &[pid, info] : players) {
    if (pid == player.getId())
      continue;
    senderQueueMonitor.sendToClient(
        player.getId(),
        InventoryUpdateEventDTO{
            pid, info->getInventoryItems(),
            info->getEquippedWeapon().getID(),
            info->getEquippedArmor().getID(),
            info->getEquippedHelmet().getID(),
            info->getEquippedShield().getID()});
  }

  for (auto &npc : npcs) {
    senderQueueMonitor.sendToClient(
        player.getId(),
        NPCAppearedEventDTO{npc->getId(), static_cast<uint8_t>(npc->getType()),
                            static_cast<int16_t>(npc->getX()),
                            static_cast<int16_t>(npc->getY())});
  }

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      senderQueueMonitor.sendToClient(
          player.getId(),
          CityEntityAppearedEventDTO{
              entity->getId(),
              static_cast<uint8_t>(entity->getCityEntityType()),
              static_cast<int16_t>(entity->getX()),
              static_cast<int16_t>(entity->getY()), entity->getDirection()});
    }
  }

  messagesToSend.push_back(player.toPlayerAppeared());

  messagesToSend.push_back(InventoryUpdateEventDTO{
      player.getId(), player.getInventoryItems(),
      player.getEquippedWeapon().getID(), player.getEquippedArmor().getID(),
      player.getEquippedHelmet().getID(), player.getEquippedShield().getID()});

  {
    std::vector<GroundItemInfoDTO> groundItemList;
    for (const auto &gi : groundItems) {
      groundItemList.push_back({gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                static_cast<int16_t>(gi.y)});
    }
    senderQueueMonitor.sendToClient(
        player.getId(), GroundItemsListEventDTO{std::move(groundItemList)});
  }
}

std::pair<int, int> PlayerService::playerSpawnPosition() {
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 2 * gridSize;
  return {spawnX, spawnY};
}

std::vector<uint32_t> PlayerService::getClanOfPlayerId(uint32_t playerId) {
  auto pOpt = getPlayer(playerId);
  if (!pOpt.has_value()) {
    return std::vector<uint32_t>();
  }
  return getClanOfPlayer(*(pOpt.value()));
}

std::vector<uint32_t> PlayerService::getClanOfPlayer(const Character &player) {
  std::vector<uint32_t> clanMembers;
  if (!player.hasClan()) {
    return clanMembers;
  }
  std::vector<std::string> members = clanManager.getMembers(player.getClanId());
  for (const std::string &playerName : members) {
    auto playerClanOpt = getPlayer(playerName);
    if (playerClanOpt.has_value()) {
      clanMembers.push_back(playerClanOpt.value()->getId());
    }
  }
  return clanMembers;
}
