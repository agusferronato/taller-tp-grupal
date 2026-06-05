#include "GameModel.h"
#include "ChatMessageEventDTO.h"
#include "GameWindow.h"
#include "InventoryUpdateEventDTO.h"
#include "NPC.h"
#include "NPCAppearedEventDTO.h"
#include "NPCType.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
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
                     Queue<ClientCommandDTO> &sendingQueue)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      myPlayerID(myPlayerID), gameView(gameView) {
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

  auto player = std::make_unique<ClientPlayer>(
      pid, appeared.playerName, appeared.x, appeared.y, appeared.direction,
      playerStatsFrom(appeared));

  if (pid == myPlayerID) {
    gameView->setMyPlayer(*player, pid);
  } else {
    gameView->addPlayer(pid, *player);
  }
  players[pid] = std::move(player);
}

void GameModel::handle(const PlayerRemovedEventDTO &removed) {
  gameView->removePlayer(removed.playerId);
  players.erase(removed.playerId);
}

void GameModel::handle(const PlayerInfoEventDTO &event) {
  auto it = players.find(event.playerId);
  if (it != players.end()) {
    it->second->updateStats(event.hp, event.maxHp, event.mana, event.maxMana,
                            event.gold, event.level, event.experience);
  }
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
        auto player = std::make_unique<ClientPlayer>(
            info.playerId, info.playerName, info.x, info.y, info.direction,
            playerStatsFrom(info));

        gameView->addPlayer(info.playerId, *player);
        players[info.playerId] = std::move(player);
      }
      break;
    } else if (auto *texInfo = std::get_if<TextureInfoEventDTO>(&event)) {
      handle(*texInfo);
    }
  }
}

void GameModel::handle(const InventoryUpdateEventDTO &) {}
void GameModel::handle(const PlayerListEventDTO &) {}
void GameModel::handle(const ChatMessageEventDTO &) {}
void GameModel::handle(const PrivateMessageEventDTO &) {}

void GameModel::handle(const GlobalChatMessageEventDTO &event) {
  chatMessages.push_back(event.playerName + ": " + event.message);

  while (chatMessages.size() > 100) {
    chatMessages.pop_front();
  }

  updateChatView();
}

void GameModel::updateChatView() {
  gameView->setChatState(chatMessages, currentChatInput, chatActive);
}

const std::deque<std::string> &GameModel::getChatMessages() const {
  return chatMessages;
}

const std::string &GameModel::getCurrentChatInput() const {
  return currentChatInput;
}

bool GameModel::isChatActive() const { return chatActive; }

void GameModel::openChat() {
  chatActive = true;
  SDL_StartTextInput();
  updateChatView();
}

void GameModel::closeChat() {
  chatActive = false;
  currentChatInput.clear();
  SDL_StopTextInput();
  updateChatView();
}

void GameModel::appendChatText(const char *text) {
  currentChatInput += text;
  updateChatView();
}

void GameModel::backspaceChat() {
  if (!currentChatInput.empty()) {
    currentChatInput.pop_back();
  }
  updateChatView();
}

void GameModel::submitChat() {
  if (currentChatInput.empty()) {
    closeChat();
    return;
  }

  sendingQueue.push(GlobalChatMessageCommandDTO{myPlayerID, currentChatInput});

  currentChatInput.clear();
  chatActive = false;
  updateChatView();
}

void GameModel::handle(const NpcDefeatedEventDTO &event) {
  gameView->removeEntity(EntityType::Npc, event.npcId);
  npcs.erase(event.npcId);
}

void GameModel::handle(const NPCAppearedEventDTO &event) {
  auto npc = std::make_unique<NPC>(event.x, event.y);
  NPCType npcType = static_cast<NPCType>(event.npcType);
  gameView->addNpc(event.npcId, *npc, npcType);
  npcs[event.npcId] = std::move(npc);
}

void GameModel::handle(const RegisterPlayerEventDTO &) {}

PlayerStatsInfo GameModel::playerStatsFrom(const PlayerInfoDTO &info) {
  PlayerStatsInfo stats{stats.health = info.hp,
                        stats.mana = info.mana,
                        stats.gold = info.gold,
                        stats.level = info.level,
                        stats.experience = info.experience,
                        stats.race = info.race,
                        stats.playerClass = info.playerClass};
  return stats;
}

PlayerStatsInfo GameModel::playerStatsFrom(const PlayerAppearedEventDTO &info) {
  PlayerStatsInfo stats{stats.health = info.hp,
                        stats.mana = info.mana,
                        stats.gold = info.gold,
                        stats.level = info.level,
                        stats.experience = info.experience,
                        stats.race = info.race,
                        stats.playerClass = info.playerClass};
  return stats;
}
