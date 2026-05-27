#include "Game.h"
#include "MoveCommandDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "command/CommandFactory.h"

Game::Game(Queue<ClientCommandDTO> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}

void Game::run() {

  ConstantRateLoop rateloop(FPS_SERVER);
  CommandFactory factory;
  unsigned int it = 0;

  while (keepRunning) {
    ClientCommandDTO dto;

    if (gameloopQueue.try_pop(dto)) {
      auto command = factory.create(std::move(dto));
      command->execute(*this);
    }
    movePlayers();
    sendMessages();

    rateloop.updateTimer(it);
  }
}

void Game::kill() { keepRunning = false; }

void Game::sendMessages() {
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

    messagesToSend.push_back(
        PlayerMovedEventDTO{playerID, static_cast<int16_t>(info.x),
                            static_cast<int16_t>(info.y), info.direction});
  }
}

void Game::registerPlayer() {
  uint32_t newId = nextPlayerId++;

  players[newId] = PlayerInfo{0, 0, Direction::Down};

  messagesToSend.push_back(RegisterPlayerEventDTO{newId, 0});

  std::vector<PlayerInfoDTO> playerList;

  for (auto &[pid, info] : players) {
    playerList.push_back({pid, static_cast<int16_t>(info.x),
                          static_cast<int16_t>(info.y), info.direction});
  }

  messagesToSend.push_back(PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(
      PlayerAppearedEventDTO{newId, 0, 0, Direction::Down});
}

void Game::movePlayer(uint32_t playerId, Direction direction) {
  auto it = players.find(playerId);

  if (it == players.end()) {
    return;
  }

  PlayerInfo &player = it->second;

  player.direction = direction;

  player.moving = true;
}

void Game::stopPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  PlayerInfo &player = it->second;
  player.moving = false;
  messagesToSend.push_back(PlayerStoppedEventDTO{playerId});
}
