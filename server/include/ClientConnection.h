#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <cstdint>
#include <memory>
#include <utility>

#include "ClientMessage.h"
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
  uint32_t connectionId;

public:
  ClientConnection(Socket &&peer, Queue<ClientMessage> &gameloopQueue,
                   SenderQueueMonitor &senderQueueMonitor,
                   uint32_t connectionId);

  ClientConnection(const ClientConnection &) = delete;
  ClientConnection &operator=(const ClientConnection &) = delete;

  uint32_t getId() const;

  void kill();

  bool isDead();

  virtual void run() override;
};

#endif
