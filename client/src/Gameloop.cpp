#include "Gameloop.h"

#include "LoginPlayerCommandDTO.h"
#include "PlayerClass.h"
#include "Race.h"
#include "RegisterPlayerCommandDTO.h"

#include <iostream>
#include <list>
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
      gameController->update();
      gameModel->updateStateFromServer();
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
  if (std::holds_alternative<ClientDataRegister>(clientData)) {
    const ClientDataRegister registerData =
        std::get<ClientDataRegister>(clientData);
    Race race = RaceUtils::stringToRace(registerData.race);
    PlayerClass playerClass =
        PlayerClassUtils::stringToPlayerClass(registerData.playerClass);
    sendingQueue.push(
        RegisterPlayerCommandDTO{registerData.username, race, playerClass});
  } else if (std::holds_alternative<ClientDataLogin>(clientData)) {
    const ClientDataLogin loginData = std::get<ClientDataLogin>(clientData);
    sendingQueue.push(LoginPlayerCommandDTO{loginData.username});
  } else {
    throw std::runtime_error("Invalid client data");
  }

  std::list<ServerEventDTO> deferredEvents;

  ServerEventDTO event;
  uint32_t myPlayerId = 0;
  bool registered = false;
  bool isLogin = std::holds_alternative<ClientDataLogin>(clientData);

  while (!registered) {
    event = receptionQueue.pop();

    if (isLogin) {
      auto *resp = std::get_if<LoginResultEventDTO>(&event);
      if (resp == nullptr) {
        deferredEvents.push_back(std::move(event));
        continue;
      }
      if (resp->status != LoginStatus::Success) {
        if (resp->status == LoginStatus::AlreadyOnline) {
          throw std::runtime_error("Ese personaje ya esta conectado");
        }
        throw std::runtime_error("No existe un personaje con ese nombre");
      }
      myPlayerId = resp->playerId;
      registered = true;
    } else if (auto *resp = std::get_if<RegisterPlayerEventDTO>(&event)) {
      if (resp->status != RegisterStatus::Success) {
        throw std::runtime_error("Player registration failed");
      }
      myPlayerId = resp->playerId;
      registered = true;
    } else {
      deferredEvents.push_back(std::move(event));
    }
  }

  gameView = std::make_unique<GameWindow>(myPlayerId);
  audio = std::make_unique<Audio>();

  gameModel = std::make_unique<GameModel>(myPlayerId, gameView.get(),
                                          receptionQueue, sendingQueue,
                                          audio.get());
  gameController = std::make_unique<GameController>(gameModel.get(), audio.get());
  audio->startMusic();

  for (auto &deferred : deferredEvents) {
    receptionQueue.push(std::move(deferred));
  }
}
