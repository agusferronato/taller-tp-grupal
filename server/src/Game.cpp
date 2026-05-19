#include "Game.h"
#include "RegisterPlayerDTO.h"
#include "MoveCommandDTO.h"
#include "RegisterPlayerResponseDTO.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include <PlayerStoppedDTO.h>

Game::Game(Queue<std::unique_ptr<CommandDTO>>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor):
        gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}

void Game::run() {

    ConstantRateLoop rateloop(FPS_SERVER);
    unsigned int it = 0;

    while (keepRunning) {

        auto command = gameloopQueue.pop();
        execute(std::move(command));

        sendMessages();

        rateloop.updateTimer(it);
    }
}

void Game::kill() { keepRunning = false; }

void Game::execute(std::unique_ptr<CommandDTO> clientMessage) {
    uint8_t code = clientMessage->getCode();

    if (code == static_cast<uint8_t>(CommandOpCode::RegisterPlayer)) {
        uint32_t newId = nextPlayerId++;
        players[newId] = PlayerInfo{0, 0, Direction::Down};

        messagesToSend.push_back(
            std::make_unique<RegisterPlayerResponseDTO>(newId, 0));

        std::vector<PlayerInfoDTO> playerList;
        for (auto& [pid, info] : players) {
            playerList.push_back({pid, static_cast<int16_t>(info.x), static_cast<int16_t>(info.y), info.direction});
        }
        messagesToSend.push_back(
            std::make_unique<PlayerListDTO>(std::move(playerList)));

        messagesToSend.push_back(
            std::make_unique<PlayerAppearedEventDTO>(newId, 0, 0, Direction::Down));

    } else if (code == static_cast<uint8_t>(CommandOpCode::MoveCommand)) {

        auto& moveCmd = dynamic_cast<MoveCommandDTO&>(*clientMessage);
        uint32_t pid = moveCmd.getPlayerId();
        Direction dir = moveCmd.getDirection();

        auto it = players.find(pid);
        if (it == players.end()) return;

        PlayerInfo& player = it->second;
        player.direction = dir;
        switch (dir) {
            case Direction::Up:    player.y -= 1; break;
            case Direction::Down:  player.y += 1; break;
            case Direction::Left:  player.x -= 1; break;
            case Direction::Right: player.x += 1; break;
        }

        std::cout << "player: " << pid << "moved to x: " << player.x << " y: " << player.y << std::endl;

        messagesToSend.push_back(
            std::make_unique<PlayerMovedEventDTO>(pid,
                static_cast<int16_t>(player.x),
                static_cast<int16_t>(player.y), dir));

    } else if (code == static_cast<uint8_t>(ServerOpcode::PlayerStopped)) {

        auto& stoppedCmd = dynamic_cast<PlayerStoppedDTO&>(*clientMessage);
        messagesToSend.push_back(std::make_unique<PlayerStoppedDTO>(stoppedCmd));
        
    }
}

void Game::sendMessages() {
    if (messagesToSend.empty()) return;
    senderQueueMonitor.broadCast(messagesToSend);
    messagesToSend.clear();
}
