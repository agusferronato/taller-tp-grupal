#include "ClientConnection.h"

ClientConnection::ClientConnection(Socket &&peer,
                                   Queue<ClientMessage> &gameloopQueue,
                                   SenderQueueMonitor &senderQueueMonitor,
                                   uint32_t connectionId)
    : peer(std::move(peer)), receiver(this->peer, gameloopQueue, connectionId),
      sender(senderQueueMonitor, this->peer, connectionId),
      connectionId(connectionId) {}

uint32_t ClientConnection::getId() const { return connectionId; }

bool ClientConnection::isDead() {
  return !receiver.is_alive() && !sender.is_alive();
}

void ClientConnection::kill() {
  receiver.kill();
  sender.kill();

  try {
    peer.shutdown(SHUT_RDWR);
    peer.close();
  } catch (...) {}
}

void ClientConnection::run() {
  receiver.start();
  sender.start();

  receiver.join();
  sender.join();
}
