#include "Game.h"
#include "MoveCommandDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "RegisterPlayerDTO.h"
#include "RegisterPlayerResponseDTO.h"
#include <PlayerStoppedDTO.h>
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include "RegisterPlayerResponseDTO.h"

Game::Game(Queue<std::unique_ptr<CommandDTO>> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}
Game::Game(Queue<std::unique_ptr<CommandDTO>> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}

void Game::run() {

  ConstantRateLoop rateloop(FPS_SERVER);
  unsigned int it = 0;
  ConstantRateLoop rateloop(FPS_SERVER);
  unsigned int it = 0;

  while (keepRunning) {
  while (keepRunning) {

    std::unique_ptr<CommandDTO> command;
    // Es un try_pop: aunque nadie mande nada, los mobs se siguen moviendo y
    // atacando, por lo que no se debería bloquear la lógica. Solo se bloquea
    // con el sleep del rateloop.
    while (gameloopQueue.try_pop(command)) {
      execute(std::move(command));
    }
    movePlayers();
    sendMessages();

    rateloop.updateTimer(it);
  }
    rateloop.updateTimer(it);
  }
}

void Game::kill() { keepRunning = false; }

void Game::execute(std::unique_ptr<CommandDTO> clientMessage) {
  uint8_t code = clientMessage->getCode();
  uint8_t code = clientMessage->getCode();

  if (code == static_cast<uint8_t>(CommandOpCode::RegisterPlayer)) {
    uint32_t newId = nextPlayerId++;
    players[newId] = PlayerInfo{0, 0, Direction::Down};
  if (code == static_cast<uint8_t>(CommandOpCode::RegisterPlayer)) {
    uint32_t newId = nextPlayerId++;
    players[newId] = PlayerInfo{0, 0, Direction::Down};

    messagesToSend.push_back(
        std::make_unique<RegisterPlayerResponseDTO>(newId, 0));
    messagesToSend.push_back(
        std::make_unique<RegisterPlayerResponseDTO>(newId, 0));

    std::vector<PlayerInfoDTO> playerList;
    for (auto &[pid, info] : players) {
      playerList.push_back({pid, static_cast<int16_t>(info.x),
                            static_cast<int16_t>(info.y), info.direction});
    }
    messagesToSend.push_back(
        std::make_unique<PlayerListDTO>(std::move(playerList)));
    std::vector<PlayerInfoDTO> playerList;
    for (auto &[pid, info] : players) {
      playerList.push_back({pid, static_cast<int16_t>(info.x),
                            static_cast<int16_t>(info.y), info.direction});
    }
    messagesToSend.push_back(
        std::make_unique<PlayerListDTO>(std::move(playerList)));

    messagesToSend.push_back(
        std::make_unique<PlayerAppearedEventDTO>(newId, 0, 0, Direction::Down));
    messagesToSend.push_back(
        std::make_unique<PlayerAppearedEventDTO>(newId, 0, 0, Direction::Down));

  } else if (code == static_cast<uint8_t>(CommandOpCode::MoveCommand)) {
  } else if (code == static_cast<uint8_t>(CommandOpCode::MoveCommand)) {

    auto &moveCmd = dynamic_cast<MoveCommandDTO &>(*clientMessage);
    uint32_t pid = moveCmd.getPlayerId();
    Direction dir = moveCmd.getDirection();
    auto &moveCmd = dynamic_cast<MoveCommandDTO &>(*clientMessage);
    uint32_t pid = moveCmd.getPlayerId();
    Direction dir = moveCmd.getDirection();

    auto it = players.find(pid);
    if (it == players.end())
      return;

    PlayerInfo &player = it->second;
    player.direction = dir;
    player.moving = true;

  } else if (code == static_cast<uint8_t>(ServerOpcode::PlayerStopped)) {

    auto &stoppedCmd = dynamic_cast<PlayerStoppedDTO &>(*clientMessage);
    uint32_t playerID = stoppedCmd.getPlayerID();

    auto it = players.find(playerID);
    if (it == players.end())
      return;

    PlayerInfo &player = it->second;
    player.moving = false;
    messagesToSend.push_back(std::make_unique<PlayerStoppedDTO>(stoppedCmd));
  }
}

void Game::sendMessages() {
  if (messagesToSend.empty())
    return;
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
  if (messagesToSend.empty())
    return;
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info.moving)
      continue;

    switch (info.direction) {
    case Direction::Up:
      info.y -= 1;
      break;
    case Direction::Down:
      info.y += 1;
      break;
    case Direction::Left:
      info.x -= 1;
      break;
    case Direction::Right:
      info.x += 1;
      break;
    }

    std::cout << "player: " << playerID << "moved to x: " << info.x
              << " y: " << info.y << std::endl;

    messagesToSend.push_back(std::make_unique<PlayerMovedEventDTO>(
        playerID, static_cast<int16_t>(info.x), static_cast<int16_t>(info.y),
        info.direction));
  }
}
