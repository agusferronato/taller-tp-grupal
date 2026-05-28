#include "Server.h"
#include "PlayerRepository.h"

Server::Server(const char *port) : socket(Socket(port)) {}

void Server::run() {

  Queue<ClientCommandDTO> gameloopQueue(GAMELOOP_QUEUE_SIZE);
  SenderQueueMonitor senderQueueMonitor;
  PlayerRepository repository(DATA_DIR);

  Acceptor acceptor(socket, gameloopQueue, senderQueueMonitor);
  acceptor.start();

  Game game(gameloopQueue, senderQueueMonitor, repository);
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
