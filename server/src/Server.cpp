#include "Server.h"

Server::Server(const char *port) : socket(Socket(port)) {}

void Server::run() {

  Queue<ClientRequestDTO> gameloopQueue(GAMELOOP_QUEUE_SIZE);
  SenderQueueMonitor senderQueueMonitor;

  Acceptor acceptor(socket, gameloopQueue, senderQueueMonitor);
  acceptor.start();

  Game game(gameloopQueue, senderQueueMonitor);
  game.start();

  std::string input;
  while (std::getline(std::cin, input)) {
    if (input == "q") {
      acceptor.kill();
      game.kill();
      break;
    }
  }

  acceptor.join();
  game.join();
}
