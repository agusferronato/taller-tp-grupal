#include "ClientConnection.h"

ClientConnection::ClientConnection(Socket &&peer,
                                   Queue<ClientCommandDTO> &gameloopQueue,
                                   SenderQueueMonitor &senderQueueMonitor)
    : peer(std::move(peer)), receiver(this->peer, gameloopQueue),
      sender(senderQueueMonitor, this->peer) {}

bool ClientConnection::isDead() {
  return !receiver.is_alive() && !sender.is_alive();
}

void ClientConnection::kill() {
  receiver.kill();
  sender.kill();

  peer.shutdown(SHUT_RDWR);
  peer.close();
}

void ClientConnection::run() {
  receiver.start();
  sender.start();

  receiver.join();
  sender.join();
}
