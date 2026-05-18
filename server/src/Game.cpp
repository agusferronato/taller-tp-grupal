#include "Game.h"
#include "RegisterPlayerDTO.h"
#include "MoveCommandDTO.h"
#include "RegisterPlayerResponseDTO.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"

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
        PlayerId newId = nextPlayerId++;
        players[newId] = PlayerInfo{0, 0, Direction::Down};

        messagesToSend.push_back(
            std::make_unique<RegisterPlayerResponseDTO>(newId, 0));

        std::vector<PlayerId> ids;
        for (auto& [pid, _] : players) {
            ids.push_back(pid);
        }
        messagesToSend.push_back(
            std::make_unique<PlayerListDTO>(std::move(ids)));

        messagesToSend.push_back(
            std::make_unique<PlayerMovedEventDTO>(newId, 0, 0, Direction::Down));

    } else if (code == static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND)) {

        auto& moveCmd = dynamic_cast<MoveCommandDTO&>(*clientMessage);
        PlayerId pid = moveCmd.getPlayerId();
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

    }
}

void Game::sendMessages() {
    if (messagesToSend.empty()) return;
    senderQueueMonitor.broadCast(messagesToSend);
    messagesToSend.clear();
}
