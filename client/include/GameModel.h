#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "ClientPlayer.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "NPC.h"
#include "Queue.h"
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

private:
  void registerPlayers();
  PlayerStatsInfo playerStatsFrom(const PlayerInfoDTO &info);
  PlayerStatsInfo playerStatsFrom(const PlayerAppearedEventDTO &info);

private:
  /* Event handlers */
  void handle(const PlayerMovedEventDTO &event);
  void handle(const PlayerAppearedEventDTO &event);
  void handle(const PlayerStoppedEventDTO &event);
  void handle(const PlayerRemovedEventDTO &event);
  void handle(const PlayerInfoEventDTO &event);
  void handle(const InventoryUpdateEventDTO &event);
  void handle(const NPCAppearedEventDTO &event);
  void handle(const TextureInfoEventDTO &event);
  void handle(const PlayerListEventDTO &event);
  void handle(const ChatMessageEventDTO &event);
  void handle(const NpcDefeatedEventDTO &event);
  void handle(const RegisterPlayerEventDTO &event);
};

#endif
