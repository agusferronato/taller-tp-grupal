#include "Gameloop.h"
#include "GameWindow.h"
#include "LoginPlayerDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include <iostream>
#include <stdexcept>

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
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

void Gameloop::makeGame(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                        Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                        const ClientData &clientData) {
  if (clientData.is_new_character) {
    sendingQueue.push(
        std::make_unique<RegisterPlayerDTO>(clientData.character_name));
  } else {
    sendingQueue.push(std::make_unique<LoginPlayerDTO>(clientData.username));
  }

  std::unique_ptr<CommandDTO> cmd;
  cmd = receptionQueue.pop();
  auto *resp = dynamic_cast<RegisterPlayerResponseDTO *>(cmd.get());
  if (!resp) {
    throw std::runtime_error("RegisterPlayerResponseDTO is null");
  }
  if (resp->getStatus() != 0) {
    throw std::runtime_error("Player registration failed");
  }

  uint32_t myPlayerId = resp->getPlayerId();

  gameView = std::make_unique<GameWindow>(myPlayerId);
  gameModel = std::make_unique<GameModel>(myPlayerId, gameView.get(),
                                          receptionQueue, sendingQueue);
  gameController = std::make_unique<EventHandler>(gameModel.get(), myPlayerId);
}
