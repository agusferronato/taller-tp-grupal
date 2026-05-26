#include "SenderQueueMonitor.h"

SenderQueueMonitor::SenderQueueMonitor() {}

Queue<ServerEventDTO> *SenderQueueMonitor::getNewSenderQueue() {
  std::lock_guard<std::mutex> lock(mutex);

  auto senderQueue = new Queue<ServerEventDTO>(SENDER_QUEUE_SIZE);
  std::queue<ServerEventDTO> pendingMessages;

  senderQueues.push_back(senderQueue);
  queuesPendingMessages.insert({senderQueue, std::move(pendingMessages)});

  return senderQueue;
}

void SenderQueueMonitor::deleteSenderQueue(Queue<ServerEventDTO> &senderQueue) {
  std::lock_guard<std::mutex> lock(mutex);

  senderQueue.close();
  senderQueues.remove(&senderQueue);
  queuesPendingMessages.erase(&senderQueue);
}

void SenderQueueMonitor::broadCast(std::list<ServerEventDTO> &messagesToSend) {
  std::lock_guard<std::mutex> lock(mutex);

  for (const auto &message : messagesToSend) {
    pushMessageToTheSenderQueues(message);
  }

  for (auto queue : senderQueues) {
    clearPendingMessages(*queue);
  }
}

void SenderQueueMonitor::clearPendingMessages(Queue<ServerEventDTO> &queue) {
  auto &pending = queuesPendingMessages[&queue];

  while (!pending.empty()) {
    if (!queue.try_push(std::move(pending.front()))) {
      break;
    }

    pending.pop();
  }
}

void SenderQueueMonitor::pushMessageToTheSenderQueues(
    const ServerEventDTO &message) {
  for (auto *queue : senderQueues) {
    queuesPendingMessages[queue].push(message);
  }
}
