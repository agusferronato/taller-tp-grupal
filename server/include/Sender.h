#ifndef SENDER_H
#define SENDER_H

#include <memory>

#include "CommunicationEnded.h"
#include "DTO/Commands/ClientRequestDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Queue.h"
#include "SenderQueueMonitor.h"
#include "Socket.h"
#include "Thread.h"
#include "protocol/Protocol.h"

class Sender : public Thread {

private:
  SenderQueueMonitor &senderQueueMonitor;
  Socket &peer;
  Queue<ServerEventDTO> *senderQueue;
  Protocol protocol;
  bool keepRunning = true;

public:
  Sender(SenderQueueMonitor &senderQueueMonitor, Socket &peer);

  Sender(const Sender &) = delete;
  Sender &operator=(const Sender &) = delete;

  void kill();
  virtual void run() override;
};

#endif
