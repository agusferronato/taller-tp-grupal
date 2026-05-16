#include "ClientSender.h"


ClientSender::ClientSender(Socket &socket,
                           Queue<Command> &sendingQueue, 
                           ShutdownEvent& shutdownEvent)
    : socket(socket),
      sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) { }


void ClientSender::run() {

  while (true) {

    try {
      Command command = sendingQueue.pop();
      protocol.send(command);

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
