#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <memory>

#include "CommunicationEnded.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "Queue.h"
#include "ShutdownEvent.h"
#include "Socket.h"
#include "Thread.h"
#include "protocol/Protocol.h"

class ClientSender : public Thread {

private:
  Socket &socket;
  Queue<ClientCommandDTO> &sendingQueue;
  ShutdownEvent &shutdownEvent;
  Protocol protocol;

public:
  ClientSender(Socket &socket, Queue<ClientCommandDTO> &sendingQueue,
               ShutdownEvent &shutdownEvent);

  virtual void run() override;
};

#endif
