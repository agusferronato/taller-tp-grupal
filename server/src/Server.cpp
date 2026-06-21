#include "Server.h"

#include <iostream>

#include "PlayerRepository.h"

Server::Server(const char *port, const char *mapPath)
    : socket(Socket(port)), mapPath(mapPath) {}

void Server::run() {

  Queue<ClientMessage> gameloopQueue;
  SenderQueueMonitor senderQueueMonitor;
  PlayerRepository repository(DATA_DIR);
  ClanManager clanManager(DATA_DIR);

  Acceptor acceptor(socket, gameloopQueue, senderQueueMonitor);
  acceptor.start();

  Game game(gameloopQueue, senderQueueMonitor, repository, clanManager,
            mapPath);
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
