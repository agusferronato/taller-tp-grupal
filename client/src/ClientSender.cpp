#include "ClientSender.h"
#include "RegisterAllParsers.h"

ClientSender::ClientSender(Socket &socket,
                           Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                           ShutdownEvent &shutdownEvent)
    : socket(socket), sendingQueue(sendingQueue), shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) {
  registerAllParsers(protocol);
}

void ClientSender::run() {

  while (true) {

    try {
      auto command = sendingQueue.pop();
      protocol.send(*command);

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
