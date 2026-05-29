#include "Receiver.h"

#include "DTO/Commands/ExitCommandDTO.h"
#include "DTO/Commands/MoveCommandDTO.h"
#include "DTO/Commands/PlayerStopCommandDTO.h"
#include "protocol/RegisterAllParsers.h"

Receiver::Receiver(Socket &peer, Queue<ClientMessage> &gameloopQueue,
                   uint32_t connectionId)
    : peer(peer), gameloopQueue(gameloopQueue), connectionId(connectionId),
      protocol(peer) {
  registerAllParsers(protocol);
}

void Receiver::kill() { keepRunning = false; }

void Receiver::run() {

  while (keepRunning) {

    try {
      auto command = protocol.receiveCommand();

      auto *move = std::get_if<MoveCommandDTO>(&command);
      if (move) {
        lastPlayerId = move->playerId;
      }
      auto *stop = std::get_if<PlayerStopCommandDTO>(&command);
      if (stop) {
        lastPlayerId = stop->playerId;
      }
      auto *exit = std::get_if<ExitCommandDTO>(&command);
      if (exit) {
        lastPlayerId = exit->playerId;
      }

      gameloopQueue.push(ClientMessage{std::move(command), connectionId});

    } catch (const CommunicationEnded &e) {
      if (keepRunning && lastPlayerId > 0) {
        gameloopQueue.push(ClientMessage{ExitCommandDTO{lastPlayerId}, lastPlayerId});
      }
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
