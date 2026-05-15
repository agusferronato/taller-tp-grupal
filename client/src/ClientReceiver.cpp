#include "ClientReceiver.h"

ClientReceiver::ClientReceiver(Socket &socket, Queue<Command> &receptionQueue) 
    : socket(socket),
      receptionQueue(receptionQueue),
      protocol(Protocol(socket)) { }



void ClientReceiver::run()
{

  while (isRunning) {
    
    try {
    
      Command command = protocol.receive();
      receptionQueue.push(command);
    
    } catch (const CommunicationEnded& e) {
     
      // todo: handle
      return;
    
    } catch (...) {
      
      return;
    }
    
  }

}
