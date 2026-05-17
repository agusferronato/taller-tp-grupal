#include "ClientSender.h"

ClientSender::ClientSender(Socket &socket,
                           Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                           ShutdownEvent& shutdownEvent)
    : socket(socket),
      sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) { }

void ClientSender::run() {

  while (true) {

    try {
      auto command = sendingQueue.pop();
      protocol.send(*command);

    } catch (const CommunicationEnded& e) {

      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      return;

    } catch (const ClosedQueue& e) {

      return;

    } catch (...) {

      return;
    }
  }
}
