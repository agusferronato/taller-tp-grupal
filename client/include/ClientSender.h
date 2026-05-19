#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <memory>

#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"
#include "Queue.h"
#include "ShutdownEvent.h"
#include "Socket.h"
#include "Thread.h"

class ClientSender : public Thread {

private:
  Socket &socket;
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  ShutdownEvent &shutdownEvent;
  Protocol protocol;

public:
  ClientSender(Socket &socket, Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
               ShutdownEvent &shutdownEvent);

  virtual void run() override;
};

#endif
