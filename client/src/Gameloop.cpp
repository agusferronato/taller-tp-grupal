#include "Gameloop.h"
#include "GameWindow.h"
#include "LoginPlayerDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                   ShutdownEvent &shutdownEvent, const ClientData &clientData)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent), controller(EventHandler(sendingQueue)),
      clientData(clientData) {
  view = std::make_unique<GameWindow>();
}

void Gameloop::run() {

  registerPlayer();

  unsigned int it = 0;

  ConstantRateLoop rateloop(FPS);

  while (!shutdownEvent.finished()) {

    try {

      controller.update(myPlayerId);
      updateStateFromServer();

      view->show(it);

    } catch (const ClosedQueue &e) {

      return;

    } catch (const WindowClosed &e) {

      shutdownEvent.put(ShutdownReason::SDLQuit);
      return;

    } catch (...) {

      return;
    }

    rateloop.updateTimer(it);
  }
}

void Gameloop::registerPlayer() {
  if (clientData.is_new_character) {
    sendingQueue.push(
        std::make_unique<RegisterPlayerDTO>(clientData.character_name));
  } else {
    sendingQueue.push(std::make_unique<LoginPlayerDTO>(clientData.username));
  }

  std::unique_ptr<CommandDTO> cmd;
  cmd = receptionQueue.pop();
  auto *resp = dynamic_cast<RegisterPlayerResponseDTO *>(cmd.get());
  if (resp && resp->getStatus() == 0) {
    myPlayerId = resp->getPlayerId();
    this->myPlayer = std::make_unique<Player>(myPlayerId, 0, 0);
    view->setMyPlayerID(myPlayerId);
    view->addPlayer(myPlayerId, myPlayer->getObserver());
  }

  cmd = receptionQueue.pop();
  auto *list = dynamic_cast<PlayerListDTO *>(cmd.get());
  if (list) {
    for (const auto &info : list->getPlayers()) {
      if (info.player_id == myPlayerId)
        continue;
      auto player = std::make_unique<Player>(info.player_id, info.x, info.y);
      otherPlayers[info.player_id] = std::move(player);
      view->addPlayer(info.player_id,
                      otherPlayers[info.player_id]->getObserver());
    }
  }
}

void Gameloop::updateStateFromServer() {

  std::unique_ptr<CommandDTO> cmd;

  while (receptionQueue.try_pop(cmd)) {

    switch (static_cast<ServerOpcode>(cmd->getCode())) {

    case ServerOpcode::PlayerMoved: {
      playerMovedHandler(cmd);
      break;
    }

    case ServerOpcode::PlayerAppeared: {
      playerAppeared(cmd);
      break;
    }

    case ServerOpcode::PlayerStopped: {
      playerStopped(cmd);
      break;
    }

    default:
      break;
    }
  }
}

void Gameloop::playerMovedHandler(std::unique_ptr<CommandDTO> &cmd) {

  auto *moved = dynamic_cast<PlayerMovedEventDTO *>(cmd.get());
  if (!moved)
    return;

  uint32_t pid = moved->getPlayerId();
  int16_t x = moved->getX();
  int16_t y = moved->getY();
  Direction dir = moved->getDirection();

  if (pid == myPlayerId) {
    myPlayer->updateCoordinates(x, y, dir);
  } else {
    auto it = otherPlayers.find(pid);
    if (it != otherPlayers.end()) {
      it->second->updateCoordinates(x, y, dir);
    }
  }
}

void Gameloop::playerStopped(std::unique_ptr<CommandDTO> &cmd) {

  auto *stopped = dynamic_cast<PlayerStoppedDTO *>(cmd.get());
  if (!stopped)
    return;

  uint32_t pid = stopped->getPlayerID();

  if (pid == myPlayerId) {
    myPlayer->stopMoving();
  } else {
    auto it = otherPlayers.find(pid);
    if (it != otherPlayers.end()) {
      it->second->stopMoving();
    }
  }
}

void Gameloop::playerAppeared(std::unique_ptr<CommandDTO> &cmd) {

  auto *appeared = dynamic_cast<PlayerAppearedEventDTO *>(cmd.get());
  if (!appeared)
    return;

  uint32_t pid = appeared->getPlayerId();
  if (pid == myPlayerId)
    return;

  auto player =
      std::make_unique<Player>(pid, appeared->getX(), appeared->getY());

  otherPlayers[pid] = std::move(player);
  view->addPlayer(pid, otherPlayers[pid]->getObserver());
}
