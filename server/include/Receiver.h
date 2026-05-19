#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>
#include <string>
#include <utility>

#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"
#include "Queue.h"
#include "Socket.h"
#include "Thread.h"

class Receiver : public Thread {

private:
  Socket &peer;
  Queue<std::unique_ptr<CommandDTO>> &gameloopQueue;
  Protocol protocol;
  bool keepRunning = true;

public:
  Receiver(Socket &peer, Queue<std::unique_ptr<CommandDTO>> &gameloopQueue);

  Receiver(const Receiver &) = delete;
  Receiver &operator=(const Receiver &) = delete;

  void kill();

  virtual void run() override;
};

#endif
