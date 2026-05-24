#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <memory>
#include <unordered_map>

#include "ClientData.h"
#include "CommandDTO.h"
#include "ConstantRateLoop.h"
#include "EventHandler.h"
#include "GameWindow.h"
#include "Player.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "Queue.h"
#include "RegisterPlayerResponseDTO.h"
#include "ShutdownEvent.h"
#include "Thread.h"
#include "WindowClosed.h"

class Gameloop : public Thread {

private:
  Queue<std::unique_ptr<CommandDTO>> &receptionQueue;
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  ShutdownEvent &shutdownEvent;
  EventHandler controller;
  ClientData clientData;
  std::unique_ptr<GameWindow> view;
  uint32_t myPlayerId{0};
  std::unique_ptr<Player> myPlayer;
  std::unordered_map<uint32_t, std::unique_ptr<Player>> otherPlayers;

public:
  Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
           Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
           ShutdownEvent &shutdownEvent, const ClientData &clientData);

  virtual void run() override;

private:
  /* Gameloop steps */
  void registerPlayer();
  void updateStateFromServer();
  void handleEvents();

private:
  /* Event handlers */
  void playerAppeared(std::unique_ptr<CommandDTO> &cmd);
  void playerMovedHandler(std::unique_ptr<CommandDTO> &cmd);
  void playerStopped(std::unique_ptr<CommandDTO> &cmd);
};

#endif
