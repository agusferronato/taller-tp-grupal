#ifndef RECEIVER_H
#define RECEIVER_H

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "ClientMessage.h"
#include "CommunicationEnded.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "Queue.h"
#include "Socket.h"
#include "Thread.h"
#include "protocol/Protocol.h"

class Receiver : public Thread {

private:
  Socket &peer;
  Queue<ClientMessage> &gameloopQueue;
  uint32_t connectionId;
  Protocol protocol;
  bool keepRunning = true;
  uint32_t lastPlayerId{0};

public:
  Receiver(Socket &peer, Queue<ClientMessage> &gameloopQueue,
           uint32_t connectionId);

  Receiver(const Receiver &) = delete;
  Receiver &operator=(const Receiver &) = delete;

  void kill();

  virtual void run() override;
};

#endif
