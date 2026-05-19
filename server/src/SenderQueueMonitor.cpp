#include "SenderQueueMonitor.h"

SenderQueueMonitor::SenderQueueMonitor() {}

Queue<std::unique_ptr<CommandDTO>> *SenderQueueMonitor::getNewSenderQueue() {
  std::lock_guard<std::mutex> lock(mutex);

  auto senderQueue = new Queue<std::unique_ptr<CommandDTO>>(SENDER_QUEUE_SIZE);
  std::queue<std::unique_ptr<CommandDTO>> pendingMessages;

  senderQueues.push_back(senderQueue);
  queuesPendingMessages.insert({senderQueue, std::move(pendingMessages)});

  return senderQueue;
}

void SenderQueueMonitor::deleteSenderQueue(
    Queue<std::unique_ptr<CommandDTO>> &senderQueue) {
  std::lock_guard<std::mutex> lock(mutex);

  senderQueue.close();
  senderQueues.remove(&senderQueue);
  queuesPendingMessages.erase(&senderQueue);
}

void SenderQueueMonitor::broadCast(
    std::list<std::unique_ptr<CommandDTO>> &messagesToSend) {
  std::lock_guard<std::mutex> lock(mutex);

  for (auto &message : messagesToSend)
    pushMessageToTheSenderQueues(std::move(message));

  for (auto queue : senderQueues)
    clearPendingMessages(*queue);
}

void SenderQueueMonitor::clearPendingMessages(
    Queue<std::unique_ptr<CommandDTO>> &queue) {
  while (!queuesPendingMessages[&queue].empty()) {
    if (!queue.try_push(std::move(queuesPendingMessages[&queue].front())))
      break;
    queuesPendingMessages[&queue].pop();
  }
}

void SenderQueueMonitor::pushMessageToTheSenderQueues(
    std::unique_ptr<CommandDTO> message) {
  for (auto it = senderQueues.begin(); it != senderQueues.end(); ++it) {
    if (std::next(it) == senderQueues.end()) {
      queuesPendingMessages[*it].push(std::move(message));
    } else {
      queuesPendingMessages[*it].push(message->clone());
    }
  }
}
