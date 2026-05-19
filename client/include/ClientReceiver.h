#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>

#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"
#include "ShutdownEvent.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"

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
