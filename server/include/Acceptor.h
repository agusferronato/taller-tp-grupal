#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ClientConnection.h"
#include "ClientMessage.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "SenderQueueMonitor.h"
#include "Socket.h"
#include "Thread.h"
#include <sys/socket.h>

class Acceptor : public Thread {

private:
  Socket &acceptor;
  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  uint32_t nextClientId{0};
  bool keepRunning = true;

public:
  Acceptor(Socket &socket, Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor);

  Acceptor(const Acceptor &) = delete;
  Acceptor &operator=(const Acceptor &) = delete;

  virtual void run() override;

  void kill();

  void reap(std::vector<std::unique_ptr<ClientConnection>> &connections);
  void clear(std::vector<std::unique_ptr<ClientConnection>> &connections);
};

#endif
