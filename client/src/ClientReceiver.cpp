#include "ClientReceiver.h"
#include "RegisterAllParsers.h"

ClientReceiver::ClientReceiver(
    Socket &socket, Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
    ShutdownEvent &shutdownEvent)
    : socket(socket), receptionQueue(receptionQueue),
      shutdownEvent(shutdownEvent), protocol(Protocol(socket)) {
  registerAllParsers(protocol);
}

void ClientReceiver::run() {

  while (true) {

    try {

      auto command = protocol.receive();
      receptionQueue.push(std::move(command));

    } catch (const CommunicationEnded &e) {

      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      receptionQueue.close();
      return;

    } catch (const ClosedQueue &e) {

      return;
    }
  }
}
