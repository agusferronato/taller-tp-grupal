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

      gameloopQueue.push(ClientMessage{std::move(command), connectionId});

    } catch (const CommunicationEnded &e) {
      if (keepRunning) {
        gameloopQueue.push(
            ClientMessage{ExitCommandDTO{}, connectionId});
      }
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
