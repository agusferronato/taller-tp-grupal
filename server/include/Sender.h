#ifndef SENDER_H
#define SENDER_H

#include <memory>

#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"
#include "SenderQueueMonitor.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"

class Sender : public Thread {

private:
  SenderQueueMonitor &senderQueueMonitor;
  Socket &peer;
  Queue<std::unique_ptr<CommandDTO>> *senderQueue;
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
