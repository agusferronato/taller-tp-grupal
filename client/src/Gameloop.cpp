#include "Gameloop.h"

#include "LoginPlayerCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"

#include <iostream>
#include <stdexcept>
#include <string>

Gameloop::Gameloop(Queue<ServerEventDTO> &receptionQueue,
                   Queue<ClientCommandDTO> &sendingQueue,
                   ShutdownEvent &shutdownEvent, const ClientData &clientData)
    : shutdownEvent(shutdownEvent), clientData(clientData) {
  makeGame(receptionQueue, sendingQueue, clientData);
}

void Gameloop::run() {

  unsigned int it = 0;

  ConstantRateLoop rateloop(FPS);

  while (!shutdownEvent.finished()) {

    try {
      gameModel->updateStateFromServer();
      gameController->update();
      gameView->show(it);

    } catch (const ClosedQueue &e) {
      return;

    } catch (const WindowClosed &e) {
      shutdownEvent.put(ShutdownReason::SDLQuit);
      return;

    } catch (const std::exception &e) {
      std::cerr << "[ERROR] Exception in gameloop: " << e.what() << std::endl;
      return;

    } catch (...) {
      std::cerr << "[ERROR] Unknown exception in gameloop" << std::endl;
      return;
    }

    rateloop.updateTimer(it);
  }
}

void Gameloop::makeGame(Queue<ServerEventDTO> &receptionQueue,
                        Queue<ClientCommandDTO> &sendingQueue,
                        const ClientData &clientData) {
  if (clientData.is_new_character) {
    sendingQueue.push(RegisterPlayerCommandDTO{clientData.character_name});
  } else {
    sendingQueue.push(LoginPlayerCommandDTO{clientData.username});
  }

  ServerEventDTO event = receptionQueue.pop();
  // si se cierra el socket el hilo reciver cierra y lanza ClosedQueue
  // debloquenado este pop

  auto *resp = std::get_if<RegisterPlayerEventDTO>(&event);
  if (!resp) {
    throw std::runtime_error("RegisterPlayerResponseDTO is null");
  }
  if (resp->status != 0) {
    throw std::runtime_error("Player registration failed");
  }

  uint32_t myPlayerId = resp->playerId;

  gameView = std::make_unique<GameWindow>(myPlayerId);
  gameModel = std::make_unique<GameModel>(myPlayerId, gameView.get(),
                                          receptionQueue, sendingQueue);
  gameController = std::make_unique<GameController>(gameModel.get());
}
