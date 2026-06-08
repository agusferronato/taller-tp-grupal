#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "CityEntityModel.h"
#include "CityEntityType.h"
#include "ClientPlayer.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "GroundItemManager.h"
#include "NPC.h"
#include "Queue.h"
#include <deque> //Double ended queue for chat messages
#include <memory>
#include <string>
#include <unordered_map>

class GameWindow;

class GameModel {
private:
  Queue<ServerEventDTO> &receptionQueue;
  Queue<ClientCommandDTO> &sendingQueue;

  uint32_t myPlayerID;
  std::unordered_map<uint32_t, std::unique_ptr<ClientPlayer>> players;
  std::unordered_map<uint32_t, std::unique_ptr<NPC>> npcs;
  std::unordered_map<uint32_t, std::unique_ptr<CityEntityModel>> cityEntities;

  GroundItemManager groundItemManager;

  std::deque<std::string> chatMessages;
  std::string currentChatInput;
  bool chatActive = false;

  GameWindow *gameView;

public:
  GameModel(uint32_t myPlayerID, GameWindow *gameView,
            Queue<ServerEventDTO> &receptionQueue,
            Queue<ClientCommandDTO> &sendingQueue);
  void updateStateFromServer();

public:
  /*update State From Controller*/
  void moveMyPlayer(Direction direction);
  void stopMyPlayer();
  void handleInventoryClick(int screenX, int screenY, uint8_t button);
  void takeItem();
  void dropItem(uint8_t slot);
  void equipItem(uint8_t slot);
  void unequipItem(uint8_t equipSlot);
  const GroundItemManager &getGroundItemManager() const {
    return groundItemManager;
  }

  void createClan(const std::string &clanName);
  void joinClan(const std::string &clanName);
  void acceptClanRequest(const std::string &playerName);
  void leaveClan();
  void reviewClan();
  void rejectClanRequest(const std::string &playerName);
  void banClanPlayer(const std::string &playerName);
  void kickClanMember(const std::string &playerName);

  void handleLeftMouseClick(int mouseX, int mouseY);
  void handleRightMouseClick(int mouseX, int mouseY);

private:
  void attack(int mouseX, int mouseY);

public:
  // Chat
  const std::deque<std::string> &getChatMessages() const;
  const std::string &getCurrentChatInput() const;
  bool isChatActive() const;

  void openChat();
  void closeChat();
  void appendChatText(const char *text);
  void backspaceChat();
  void submitChat();
  void scrollChatUp();
  void scrollChatDown();
  int getChatScrollOffset() const;

private:
  void registerPlayers();
  PlayerStatsInfo playerStatsFrom(const PlayerInfoDTO &info);
  PlayerStatsInfo playerStatsFrom(const PlayerAppearedEventDTO &info);
  void updateChatView();

private:
  /* Event handlers */
  void handle(const PlayerMovedEventDTO &event);
  void handle(const PlayerAppearedEventDTO &event);
  void handle(const PlayerStoppedEventDTO &event);
  void handle(const PlayerRemovedEventDTO &event);
  void handle(const CityEntityAppearedEventDTO &event);
  void handle(const CityEntityMovedEventDTO &event);
  void handle(const CityEntityStoppedEventDTO &event);
  void handle(const PlayerInfoEventDTO &event);
  void handle(const InventoryUpdateEventDTO &event);
  void handle(const NPCAppearedEventDTO &event);
  void handle(const TextureInfoEventDTO &event);
  void handle(const PlayerListEventDTO &event);
  void handle(const ChatMessageEventDTO &event);
  void handle(const NPCMovedEventDTO &event);
  void handle(const NPCStoppedEventDTO &event);
  void handle(const NpcDefeatedEventDTO &event);
  void handle(const RegisterPlayerEventDTO &event);
  void handle(const GroundItemAppearedEventDTO &event);
  void handle(const GroundItemRemovedEventDTO &event);
  void handle(const GroundItemsListEventDTO &event);
  void handle(const PrivateMessageEventDTO &event);
  void handle(const GlobalChatMessageEventDTO &event);
  void handle(const PlayerDieEventDTO &event);
  void handle(const LoginResultEventDTO &event);
};

#endif
