#include "Game.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include "command/CommandFactory.h"

Game::Game(Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}

void Game::run() {
  MapLoader mapLoader("map.toml");
  maxSize = mapLoader.GetMaxSize();
  gridSize = mapLoader.GetGridSize();
  commonGroundTextureId = mapLoader.GetCommonGroundTextureId();
  textureOrigins = mapLoader.GetTextureOrigins();

  ConstantRateLoop rateloop(FPS_SERVER);
  CommandFactory factory;
  unsigned int it = 0;

  while (keepRunning) {
    ClientMessage msg;

    if (gameloopQueue.try_pop(msg)) {
      auto command = factory.create(msg.dto);
      command->execute(*this, msg.connectionId);
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

    messagesToSend.push_back(
        PlayerMovedEventDTO{playerID, static_cast<int16_t>(info.x),
                            static_cast<int16_t>(info.y), info.direction});
  }
}

void Game::registerPlayer(uint32_t connectionId) {

  players[connectionId] = PlayerInfo{0, 0, Direction::Down};

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{connectionId, 0});

  {
    std::vector<TextureOriginDTO> origins;
    origins.reserve(textureOrigins.size());
    for (const auto &o : textureOrigins) {
      origins.push_back(
          {static_cast<uint8_t>(o.priority),
           static_cast<uint8_t>(o.texture_id),
           static_cast<uint16_t>(o.x),
           static_cast<uint16_t>(o.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId,
        TextureInfoEventDTO{static_cast<uint16_t>(maxSize),
                            static_cast<uint16_t>(gridSize),
                            static_cast<uint8_t>(commonGroundTextureId),
                            std::move(origins)});
  }

  std::vector<PlayerInfoDTO> playerList;

  for (auto &[pid, info] : players) {
    playerList.push_back({pid, static_cast<int16_t>(info.x),
                          static_cast<int16_t>(info.y), info.direction});
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(
      PlayerAppearedEventDTO{connectionId, 0, 0, Direction::Down});
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
