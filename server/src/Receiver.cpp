#include "Receiver.h"
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
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
