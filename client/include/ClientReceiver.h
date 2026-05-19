#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>

#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"
#include "Queue.h"
#include "ShutdownEvent.h"
#include "Socket.h"
#include "Thread.h"

class ClientReceiver : public Thread {

private:
  Socket &socket;
  Queue<std::unique_ptr<CommandDTO>> &receptionQueue;
  ShutdownEvent &shutdownEvent;
  Protocol protocol;

public:
  ClientReceiver(Socket &socket,
                 Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                 ShutdownEvent &shutdownEvent);

  virtual void run() override;
};

#endif
