#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <memory>
#include <utility>

#include "DTO/Commands/ClientCommandDTO.h"
#include "Receiver.h"
#include "Sender.h"
#include "SenderQueueMonitor.h"
#include "Socket.h"
#include "Thread.h"
#include <sys/socket.h>

class ClientConnection : public Thread {

private:
  Socket peer;
  Receiver receiver;
  Sender sender;

public:
  ClientConnection(Socket &&peer, Queue<ClientCommandDTO> &gameloopQueue,
                   SenderQueueMonitor &senderQueueMonitor);

  ClientConnection(const ClientConnection &) = delete;
  ClientConnection &operator=(const ClientConnection &) = delete;

  void kill();

  bool isDead();

  virtual void run() override;
};

#endif
