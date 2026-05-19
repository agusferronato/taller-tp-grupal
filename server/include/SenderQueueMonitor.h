#ifndef SENDER_QUEUE_MONITOR_H
#define SENDER_QUEUE_MONITOR_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include "CommandDTO.h"
#include "Constants.h"
#include "Queue.h"

class SenderQueueMonitor {

private:
  std::mutex mutex;
  std::list<Queue<std::unique_ptr<CommandDTO>> *> senderQueues;
  std::map<Queue<std::unique_ptr<CommandDTO>> *,
           std::queue<std::unique_ptr<CommandDTO>>>
      queuesPendingMessages;

public:
  SenderQueueMonitor();

  SenderQueueMonitor(const SenderQueueMonitor &) = delete;
  SenderQueueMonitor &operator=(const SenderQueueMonitor &) = delete;

  Queue<std::unique_ptr<CommandDTO>> *getNewSenderQueue();
  void deleteSenderQueue(Queue<std::unique_ptr<CommandDTO>> &senderQueue);

  void broadCast(std::list<std::unique_ptr<CommandDTO>> &pendingMessages);

private:
  void pushMessageToTheSenderQueues(std::unique_ptr<CommandDTO> message);
  void clearPendingMessages(Queue<std::unique_ptr<CommandDTO>> &queue);
};

#endif
