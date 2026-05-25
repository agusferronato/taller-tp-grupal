#include "Receiver.h"

#include "DTO/Commands/ExitDTO.h"
#include "DTO/Commands/MoveCommandDTO.h"
#include "DTO/Commands/PlayerStopDTO.h"
#include "protocol/RegisterAllParsers.h"

Receiver::Receiver(Socket &peer, Queue<ClientRequestDTO> &gameloopQueue)
    : peer(peer), gameloopQueue(gameloopQueue), protocol(peer) {
  registerAllParsers(protocol);
}

void Receiver::kill() { keepRunning = false; }

void Receiver::run() {

  while (keepRunning) {

    try {
      auto command = protocol.receiveCommand();

      auto *move = std::get_if<MoveCommandDTO>(&command);
      if (move) {
        lastPlayerId = move->player_id;
      }
      auto *stop = std::get_if<PlayerStopDTO>(&command);
      if (stop) {
        lastPlayerId = stop->player_id;
      }
      auto *exit = std::get_if<ExitDTO>(&command);
      if (exit) {
        lastPlayerId = exit->playerId;
      }

      gameloopQueue.push(std::move(command));

    } catch (const CommunicationEnded &e) {
      if (keepRunning && lastPlayerId > 0) {
        gameloopQueue.push(ExitDTO{lastPlayerId});
      }
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
