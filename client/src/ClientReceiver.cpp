#include "ClientReceiver.h"
#include "protocol/RegisterAllParsers.h"

ClientReceiver::ClientReceiver(Socket &socket,
                               Queue<ServerEventDTO> &receptionQueue,
                               ShutdownEvent &shutdownEvent)
    : socket(socket), receptionQueue(receptionQueue),
      shutdownEvent(shutdownEvent), protocol(Protocol(socket)) {
  registerAllParsers(protocol);
}

void ClientReceiver::run() {

  while (true) {

    try {

      auto event = protocol.receiveEvent();
      receptionQueue.push(std::move(event));

    } catch (const CommunicationEnded &e) {

      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      receptionQueue.close();
      return;

    } catch (const ClosedQueue &e) {

      return;
    }
  }
}
