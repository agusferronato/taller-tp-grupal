#include "ClientReceiver.h"

ClientReceiver::ClientReceiver(Socket &socket,
                               Queue<Command> &receptionQueue, 
                               ShutdownEvent &shutdownEvent)
    : socket(socket),
      receptionQueue(receptionQueue),
      shutdownEvent(shutdownEvent),
      protocol(Protocol(socket)) { }


void ClientReceiver::run()
{

  while (true) {

    try {

      Command command = protocol.receive();
      receptionQueue.push(command);

    } catch (const CommunicationEnded& e) {
      
      shutdownEvent.put(ShutdownReason::ConnectionClosed);
      return;

    } catch (const ClosedQueue& e) {

      return;

    } 

  }

}
