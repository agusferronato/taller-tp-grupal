#include "ClientReceiver.h"
#include "protocol/RegisterAllParsers.h"

ClientReceiver::ClientReceiver(Socket &socket,
                               Queue<ServerEventDTO> &receptionQueue)
    : socket(socket), receptionQueue(receptionQueue),
      protocol(Protocol(socket)) {
  registerAllParsers(protocol);
}

void ClientReceiver::run() {

  while (true) {

    try {

      auto event = protocol.receiveEvent();
      receptionQueue.push(std::move(event));

    } catch (const CommunicationEnded &e) {

      receptionQueue.close();
      return;

    } catch (const ClosedQueue &e) {

      return;
    }
  }
}
