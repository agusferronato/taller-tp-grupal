#include "GameModel.h"
#include "ChatMessageEventDTO.h"
#include "EntityType.h"
#include "GameWindow.h"
#include "InventoryUpdateEventDTO.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerEntity.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "Race.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include <iostream>
#include <stdexcept>

GameModel::GameModel(uint32_t myPlayerID, GameWindow *gameView,
                     Queue<ServerEventDTO> &receptionQueue,
                     Queue<ClientCommandDTO> &sendingQueue,
                     TextureManager &textureManager, const std::string &race)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      myPlayerID(myPlayerID), gameView(gameView),
      textureManager(textureManager) {
  auto myPlayer = std::make_unique<Player>(this->myPlayerID, 0, 0);
  myPlayer->setRace(RaceUtils::stringToRace(race));
  players[myPlayerID] = std::move(myPlayer);

  auto entity = std::make_unique<PlayerEntity>(
      *players[myPlayerID], textureManager, gameView->getFont());
  gameView->setMyPlayer(entity.get());
  gameView->addEntity(EntityType::Player, myPlayerID, std::move(entity));

  registerPlayers();
}

void GameModel::updateStateFromServer() {
  ServerEventDTO event;

  while (receptionQueue.try_pop(event)) {
    std::visit([this](const auto &e) { handle(e); }, event);
  }
}

void GameModel::moveMyPlayer(Direction direction) {
  sendingQueue.push(MoveCommandDTO{myPlayerID, direction});
}

void GameModel::stopMyPlayer() {
  sendingQueue.push(PlayerStopCommandDTO{myPlayerID});
}

void GameModel::handle(const PlayerMovedEventDTO &moved) {
  uint32_t pid = moved.playerId;
  int16_t x = moved.x;
  int16_t y = moved.y;
  Direction dir = moved.direction;

  auto it = players.find(pid);
  if (it != players.end()) {
    it->second->updateCoordinates(x, y, dir);
  }
}

void GameModel::handle(const PlayerStoppedEventDTO &stopped) {
  auto it = players.find(stopped.playerId);
  if (it != players.end()) {
    it->second->stopMoving();
  }
}

void GameModel::handle(const PlayerAppearedEventDTO &appeared) {
  uint32_t pid = appeared.playerId;

  auto applyStats = [&](Player *p) {
    p->setName(appeared.playerName);
    p->setHp(appeared.hp);
    p->setMaxHp(appeared.maxHp);
    p->setMana(appeared.mana);
    p->setMaxMana(appeared.maxMana);
    p->setGold(appeared.gold);
    p->setLevel(appeared.level);
    p->setExperience(appeared.experience);
  };

  if (pid == myPlayerID) {
    return;
  }

  auto player = std::make_unique<Player>(pid, appeared.x, appeared.y);
  player->setRace(appeared.race);
  applyStats(player.get());

  auto entity = std::make_unique<PlayerEntity>(*player, textureManager,
                                               gameView->getFont());
  gameView->addEntity(EntityType::Player, pid, std::move(entity));
  players[pid] = std::move(player);
}

void GameModel::handle(const PlayerRemovedEventDTO &removed) {
  gameView->removeEntity(EntityType::Player, removed.playerId);
  players.erase(removed.playerId);
}

void GameModel::handle(const PlayerInfoEventDTO &info) {
  auto it = players.find(info.playerId);
  if (it == players.end()) {
    return;
  }
  it->second->setHp(info.hp);
  it->second->setMaxHp(info.maxHp);
  it->second->setMana(info.mana);
  it->second->setMaxMana(info.maxMana);
  it->second->setGold(info.gold);
  it->second->setLevel(info.level);
  it->second->setExperience(info.experience);
}

void GameModel::handle(const TextureInfoEventDTO &texInfo) {
  std::list<TileOrigin> origins;
  for (const auto &o : texInfo.origins) {
    origins.push_back({o.priority, o.texture_id, static_cast<int>(o.i),
                       static_cast<int>(o.j)});
  }
  gameView->setMapData(texInfo.maxSize, texInfo.gridSize,
                       texInfo.commonGroundTextureId, origins);
}

void GameModel::registerPlayers() {
  while (true) {
    auto event = receptionQueue.pop();

    if (auto *list = std::get_if<PlayerListEventDTO>(&event)) {
      for (const auto &info : list->players) {
        if (info.playerId == myPlayerID) {
          continue;
        }
        auto player = std::make_unique<Player>(info.playerId, info.x, info.y);
        player->setRace(info.race);
        player->setName(info.playerName);
        player->setHp(info.hp);
        player->setMaxHp(info.maxHp);
        player->setMana(info.mana);
        player->setMaxMana(info.maxMana);
        player->setGold(info.gold);
        player->setLevel(info.level);
        player->setExperience(info.experience);

        auto entity = std::make_unique<PlayerEntity>(*player, textureManager,
                                                     gameView->getFont());
        gameView->addEntity(EntityType::Player, info.playerId,
                            std::move(entity));
        players[info.playerId] = std::move(player);
      }
      break;

    } else if (auto *texInfo = std::get_if<TextureInfoEventDTO>(&event)) {
      handle(*texInfo);
    }
  }
}

void GameModel::handle(const InventoryUpdateEventDTO &inv) {
  if (inv.playerId != myPlayerID)
    return;
  auto it = players.find(inv.playerId);
  if (it == players.end())
    return;
  it->second->setInventory(inv.items);
  it->second->setEquippedWeapon(inv.equippedWeapon);
  it->second->setEquippedArmor(inv.equippedArmor);
  it->second->setEquippedHelmet(inv.equippedHelmet);
  it->second->setEquippedShield(inv.equippedShield);
}

void GameModel::handle(const PlayerListEventDTO &) {}
void GameModel::handle(const ChatMessageEventDTO &) {}
void GameModel::handle(const NpcDefeatedEventDTO &) {}
void GameModel::handle(const RegisterPlayerEventDTO &) {}
