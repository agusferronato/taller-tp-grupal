#include "ClientSender.h"


ClientSender::ClientSender(Socket &socket, Queue<Command> &sendingQueue) 
    : socket(socket),
      sendingQueue(sendingQueue),
      protocol(Protocol(socket)) { }


void ClientSender::run()
{

  while (isRunning) {

    try {
      Command command = sendingQueue.pop();
      protocol.send(command);

    } catch (const CommunicationEnded& e) {
      
      // todo: handle
      return;
    
    } catch (...) {
      
      return;
    }

}
