#include "Client.h"


Client::Client(const char *hostname, const char *port) 
    : socket(Socket(hostname, port)) { }



void Client::run() {
    
  Queue<Command> sendingQueue, receptionQueue;

  GameLoop gameloop(receptionQueue, sendingQueue);
  RateLoop loop(gameloop);

  ClientReceiver receiver(socket, receptionQueue);
  ClientSender sender(socket, sendingQueue);

  loop.start();
  receiver.start();
  sender.start();
  

}
