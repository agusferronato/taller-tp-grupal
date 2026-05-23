#include "Receiver.h"
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
      gameloopQueue.push(std::move(command));

    } catch (const CommunicationEnded &e) {
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
