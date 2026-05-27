#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>

#include "CommunicationEnded.h"
#include "DTO/Events/EventDTO.h"
#include "Queue.h"
#include "ShutdownEvent.h"
#include "Socket.h"
#include "Thread.h"
#include "protocol/Protocol.h"

class ClientReceiver : public Thread {

private:
  Socket &socket;
  Queue<ServerEventDTO> &receptionQueue;
  ShutdownEvent &shutdownEvent;
  Protocol protocol;

public:
  ClientReceiver(Socket &socket, Queue<ServerEventDTO> &receptionQueue,
                 ShutdownEvent &shutdownEvent);

  virtual void run() override;
};

#endif
