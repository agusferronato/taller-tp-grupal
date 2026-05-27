#ifndef SENDER_QUEUE_MONITOR_H
#define SENDER_QUEUE_MONITOR_H

#include <list>
#include <map>
#include <mutex>
#include <queue>

#include "Constants.h"
#include "DTO/Events/EventDTO.h"
#include "Queue.h"

class SenderQueueMonitor {
private:
  std::mutex mutex;
  std::list<Queue<ServerEventDTO> *> senderQueues;
  std::map<Queue<ServerEventDTO> *, std::queue<ServerEventDTO>>
      queuesPendingMessages;

public:
  SenderQueueMonitor();

  SenderQueueMonitor(const SenderQueueMonitor &) = delete;
  SenderQueueMonitor &operator=(const SenderQueueMonitor &) = delete;

  Queue<ServerEventDTO> *getNewSenderQueue();
  void deleteSenderQueue(Queue<ServerEventDTO> &senderQueue);

  void broadCast(std::list<ServerEventDTO> &pendingMessages);

private:
  void pushMessageToTheSenderQueues(const ServerEventDTO &message);
  void clearPendingMessages(Queue<ServerEventDTO> &queue);
};

#endif
