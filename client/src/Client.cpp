#include "Client.h"


Client::Client(const char *hostname, const char *port)
    : socket(Socket(hostname, port)) { }


void Client::run() {

  ShutdownEvent shutdownEvent;
  Queue<Command> sendingQueue, receptionQueue;
  GameLoop game(receptionQueue, sendingQueue, shutdownEvent);
  RateLoop rateloop(game, shutdownEvent);
  Connection connection(receptionQueue, sendingQueue, socket, shutdownEvent);

  connection.start();
  rateloop.start();

  shutdownEvent.wait();

  switch (shutdownEvent.getReason()) {

    case ShutdownReason::SDLQuit:
      connection.close();
      [[fallthrough]];

    case ShutdownReason::ConnectionClosed:
      sendingQueue.close();
      receptionQueue.close();

    default:
      break;
  }

  rateloop.join();
  connection.join();

}
