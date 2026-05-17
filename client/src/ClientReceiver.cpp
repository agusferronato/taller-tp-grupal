#include "ClientReceiver.h"

ClientReceiver::ClientReceiver(Socket &socket,
                               Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                               ShutdownEvent &shutdownEvent)
    : socket(socket),
      receptionQueue(receptionQueue),
      shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) { }

void ClientReceiver::run() {

  while (true) {

    try {

      auto command = protocol.receive();
      receptionQueue.push(std::move(command));

    } catch (const CommunicationEnded& e) {

      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      return;

    } catch (const ClosedQueue& e) {

      return;
    }
  }
}
