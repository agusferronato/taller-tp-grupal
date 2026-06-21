#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <memory>
#include <unordered_map>

#include "Audio.h"
#include "ClientData.h"
#include "ConstantRateLoop.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "GameController.h"
#include "GameModel.h"
#include "GameWindow.h"
#include "Player.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "Queue.h"
#include "RegisterPlayerEventDTO.h"
#include "ShutdownReason.h"
#include "Thread.h"
#include "WindowClosed.h"

class Gameloop {

private:
  ClientData clientData;

  std::unique_ptr<Audio> audio;
  std::unique_ptr<GameController> gameController;
  std::unique_ptr<GameWindow> gameView;
  std::unique_ptr<GameModel> gameModel;

public:
  Gameloop(Queue<ServerEventDTO> &receptionQueue,
           Queue<ClientCommandDTO> &sendingQueue, const ClientData &clientData);

  ShutdownReason run();

private:
  void makeGame(Queue<ServerEventDTO> &receptionQueue,
                Queue<ClientCommandDTO> &sendingQueue,
                const ClientData &clientData);
};

#endif
