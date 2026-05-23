#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "ClientConnection.h"
#include "DTO/Commands/ClientRequestDTO.h"
#include "SenderQueueMonitor.h"
#include "Socket.h"
#include "Thread.h"
#include <sys/socket.h>

class Acceptor : public Thread {

private:
  Socket &acceptor;
  Queue<ClientRequestDTO> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  bool keepRunning = true;

public:
  Acceptor(Socket &socket, Queue<ClientRequestDTO> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor);

  Acceptor(const Acceptor &) = delete;
  Acceptor &operator=(const Acceptor &) = delete;

  virtual void run() override;

  void kill();

  void reap(std::vector<ClientConnection *> &connections);
  void clear(std::vector<ClientConnection *> &connections);
};

#endif
