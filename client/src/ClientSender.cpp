#include "ClientSender.h"
#include "protocol/RegisterAllParsers.h"

ClientSender::ClientSender(Socket &socket,
                           Queue<ClientCommandDTO> &sendingQueue,
                           ShutdownEvent &shutdownEvent)
    : socket(socket), sendingQueue(sendingQueue), shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) {
  registerAllParsers(protocol);
}

void ClientSender::run() {

  while (true) {

    try {
      auto command = sendingQueue.pop();
      protocol.sendCommand(command);

    } catch (const CommunicationEnded &e) {

      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      sendingQueue.close();
      return;

    } catch (const ClosedQueue &e) {

      return;

    } catch (...) {

      return;
    }
  }
}
