#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>
#include <string>
#include <utility>

#include "CommunicationEnded.h"
#include "DTO/Commands/ClientRequestDTO.h"
#include "Queue.h"
#include "Socket.h"
#include "Thread.h"
#include "protocol/Protocol.h"

class Receiver : public Thread {

private:
  Socket &peer;
  Queue<ClientRequestDTO> &gameloopQueue;
  Protocol protocol;
  bool keepRunning = true;

public:
  Receiver(Socket &peer, Queue<ClientRequestDTO> &gameloopQueue);

  Receiver(const Receiver &) = delete;
  Receiver &operator=(const Receiver &) = delete;

  void kill();

  virtual void run() override;
};

#endif
