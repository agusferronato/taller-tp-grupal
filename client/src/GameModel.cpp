#include "GameModel.h"
#include "AcceptClanRequestCommandDTO.h"
#include "BanClanPlayerCommandDTO.h"
#include "ChatMessageEventDTO.h"
#include "CreateClanCommandDTO.h"
#include "DropItemCommandDTO.h"
#include "EquipCommandDTO.h"
#include "GameWindow.h"
#include "UnequipCommandDTO.h"
#include "GlobalChatMessageCommandDTO.h"
#include "GlobalChatMessageEventDTO.h"
#include "GroundItemAppearedEventDTO.h"
#include "GroundItemRemovedEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "JoinClanCommandDTO.h"
#include "KickClanMemberCommandDTO.h"
#include "LeaveClanCommandDTO.h"
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
#include "PrivateMessageEventDTO.h"
#include "Race.h"
#include "RegisterPlayerEventDTO.h"
#include "RejectClanRequestCommandDTO.h"
#include "ReviewClanCommandDTO.h"
#include "TextureInfoEventDTO.h"
#include <iostream>
#include <stdexcept>

static constexpr int MAX_NUMBER_OF_MESSAGES = 100;


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

  gameView->updateGroundItems(groundItemManager.getAll());
}

void GameModel::handleInventoryClick(int screenX, int screenY,
                                     uint8_t button) {
  ClickTarget target = gameView->hitTestInventory(screenX, screenY);
  if (target.type == ClickTargetType::None)
    return;

  if (target.type == ClickTargetType::Equipment) {
    sendingQueue.push(UnequipCommandDTO{myPlayerID,
                                        static_cast<uint8_t>(target.index)});
  } else if (target.type == ClickTargetType::Inventory) {
    if (button == SDL_BUTTON_RIGHT) {
      sendingQueue.push(DropItemCommandDTO{myPlayerID,
                                           static_cast<uint8_t>(target.index)});
    } else {
      sendingQueue.push(EquipCommandDTO{myPlayerID,
                                        static_cast<uint8_t>(target.index)});
    }
  }
}

void GameModel::takeItem() {
  sendingQueue.push(TakeItemCommandDTO{myPlayerID});
}

void GameModel::dropItem(uint8_t slot) {
  sendingQueue.push(DropItemCommandDTO{myPlayerID, slot});
}

void GameModel::equipItem(uint8_t slot) {
  sendingQueue.push(EquipCommandDTO{myPlayerID, slot});
}

void GameModel::unequipItem(uint8_t equipSlot) {
  sendingQueue.push(UnequipCommandDTO{myPlayerID, equipSlot});
}

void GameModel::createClan(const std::string &clanName) {
  sendingQueue.push(CreateClanCommandDTO{myPlayerID, clanName});
}

void GameModel::joinClan(const std::string &clanName) {
  sendingQueue.push(JoinClanCommandDTO{myPlayerID, clanName});
}

void GameModel::acceptClanRequest(const std::string &playerName) {
  sendingQueue.push(AcceptClanRequestCommandDTO{myPlayerID, playerName});
}

void GameModel::leaveClan() {
  sendingQueue.push(LeaveClanCommandDTO{myPlayerID});
}

void GameModel::reviewClan() {
  sendingQueue.push(ReviewClanCommandDTO{myPlayerID});
}

void GameModel::rejectClanRequest(const std::string &playerName) {
  sendingQueue.push(RejectClanRequestCommandDTO{myPlayerID, playerName});
}

void GameModel::banClanPlayer(const std::string &playerName) {
  sendingQueue.push(BanClanPlayerCommandDTO{myPlayerID, playerName});
}

void GameModel::kickClanMember(const std::string &playerName) {
  sendingQueue.push(KickClanMemberCommandDTO{myPlayerID, playerName});
}

void GameModel::moveMyPlayer(Direction direction) {
  sendingQueue.push(MoveCommandDTO{myPlayerID, direction});
}

void GameModel::stopMyPlayer() {
  sendingQueue.push(PlayerStopCommandDTO{myPlayerID});
}

void GameModel::atack(int mouseX, int mouseY) {
  auto [worldX, worldY] = gameView->screenToWorld(mouseX, mouseY);
  sendingQueue.push(AtackCommandDTO{myPlayerID, static_cast<int16_t>(worldX),
                                    static_cast<int16_t>(worldY)});
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

void GameModel::handle(const InventoryUpdateEventDTO &inv) {
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
void GameModel::handle(const ChatMessageEventDTO &event) {
  chatMessages.push_back(event.message);
  while (chatMessages.size() > 100) {
    chatMessages.pop_front();
  }
  updateChatView();
}
void GameModel::handle(const PrivateMessageEventDTO &) {}

void GameModel::handle(const GlobalChatMessageEventDTO &event) {
  chatMessages.push_back(event.playerName + ": " + event.message);

  while (chatMessages.size() > MAX_NUMBER_OF_MESSAGES) {
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
void GameModel::handle(const GroundItemAppearedEventDTO &e) {
  groundItemManager.add(e.groundItemId, e.itemId, e.x, e.y);
}
void GameModel::handle(const GroundItemRemovedEventDTO &e) {
  groundItemManager.remove(e.groundItemId);
}
void GameModel::handle(const GroundItemsListEventDTO &e) {
  groundItemManager.setAll(e.items);
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

void GameModel::scrollChatUp() {
  gameView->scrollChatUp();
  updateChatView();
}

void GameModel::scrollChatDown() {
  gameView->scrollChatDown();
  updateChatView();
}
void GameModel::handleLeftMouseClick(int mouseX, int mouseY) {
  atack(mouseX, mouseY);
}

void GameModel::handleRightMouseClick(int mouseX, int mouseY) {
  (void)mouseX;
  (void)mouseY;
}
