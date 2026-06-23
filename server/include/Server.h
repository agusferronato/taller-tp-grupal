#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <utility>

#include "Acceptor.h"
#include "Constants.h"
#include "Game.h"

class Server {

private:
  Socket socket;
  std::string mapPath;

public:
  Server(const char *port, const char *mapPath);

  Server(const Server &) = delete;
  Server &operator=(const Server &) = delete;

  void run();
};

#endif /* SERVER_H */
