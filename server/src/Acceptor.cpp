#include "Acceptor.h"

Acceptor::Acceptor(Socket &socket, Queue<ClientMessage> &gameloopQueue,
                   SenderQueueMonitor &senderQueueMonitor)
    : acceptor(socket), gameloopQueue(gameloopQueue),
      senderQueueMonitor(senderQueueMonitor) {}

void Acceptor::run() {

  std::vector<std::unique_ptr<ClientConnection>> connections;

  while (keepRunning) {

    try {
      Socket peer = acceptor.accept();

      uint32_t clientId = nextClientId++;
      auto connection = std::make_unique<ClientConnection>(
          std::move(peer), gameloopQueue, senderQueueMonitor, clientId);

      reap(connections);
      connection->run();
      connections.push_back(std::move(connection));

    } catch (...) {
      break;
    }
  }
  clear(connections);
}

void Acceptor::kill() {
  keepRunning = false;

  acceptor.shutdown(SHUT_RDWR);
  acceptor.close();
}

void Acceptor::reap(
    std::vector<std::unique_ptr<ClientConnection>> &connections) {

  auto it = connections.begin();

  while (it != connections.end()) {
    ClientConnection &client = **it;

    if (client.isDead()) {
      client.join();
      it = connections.erase(it);
    } else {
      ++it;
    }
  }
}

void Acceptor::clear(
    std::vector<std::unique_ptr<ClientConnection>> &connections) {

  for (auto &client : connections) {
    try {
      client->kill();
    } catch (...) {
    }

    client->join();
  }

  connections.clear();
}
