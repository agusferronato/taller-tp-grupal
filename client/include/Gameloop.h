#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <memory>
#include <unordered_map>

#include "ClientData.h"
#include "CommandDTO.h"
#include "ConstantRateLoop.h"
#include "EventHandler.h"
#include "GameModel.h"
#include "GameWindow.h"
#include "Player.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "Queue.h"
#include "RegisterPlayerResponseDTO.h"
#include "ShutdownEvent.h"
#include "Thread.h"
#include "WindowClosed.h"

class Gameloop {

private:
  ShutdownEvent &shutdownEvent;
  std::unique_ptr<EventHandler> gameController;
  ClientData clientData;
  std::unique_ptr<GameWindow> gameView;
  std::unique_ptr<GameModel> gameModel;

public:
  Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
           Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
           ShutdownEvent &shutdownEvent, const ClientData &clientData);

  void run();

private:
  void makeGame(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                const ClientData &clientData);
};

#endif
