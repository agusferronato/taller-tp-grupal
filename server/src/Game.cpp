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

  switch (direction) {
  case Direction::Up:
    player.y -= 1;
    break;

  case Direction::Down:
    player.y += 1;
    break;

  case Direction::Left:
    player.x -= 1;
    break;

  case Direction::Right:
    player.x += 1;
    break;
  }

  std::cout << "player: " << playerId << " moved to x: " << player.x
            << " y: " << player.y << std::endl;

  messagesToSend.push_back(
      PlayerMovedEventDTO{playerId, static_cast<int16_t>(player.x),
                          static_cast<int16_t>(player.y), direction});
}

void Game::stopPlayer(uint32_t playerId) {
  messagesToSend.push_back(PlayerStoppedEventDTO{playerId});
}
