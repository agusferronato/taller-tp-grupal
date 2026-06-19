#include "SenderQueueMonitor.h"

SenderQueueMonitor::SenderQueueMonitor() {}

Queue<ServerEventDTO> *
SenderQueueMonitor::getNewSenderQueue(uint32_t clientId) {
  std::lock_guard<std::mutex> lock(mutex);

  auto senderQueue = new Queue<ServerEventDTO>(SENDER_QUEUE_SIZE);

  senderQueues[clientId] = senderQueue;
  queuesPendingMessages[clientId] = {};

  return senderQueue;
}

void SenderQueueMonitor::deleteSenderQueue(uint32_t clientId) {
  std::lock_guard<std::mutex> lock(mutex);

  auto it = senderQueues.find(clientId);
  if (it == senderQueues.end())
    return;

  it->second->close();
  senderQueues.erase(clientId);
  queuesPendingMessages.erase(clientId);
  delete it->second;
}

void SenderQueueMonitor::broadCast(std::list<ServerEventDTO> &messagesToSend) {
  std::lock_guard<std::mutex> lock(mutex);

  for (const auto &message : messagesToSend) {
    pushMessageToTheSenderQueues(message);
  }

  for (const auto &[id, _] : senderQueues) {
    clearPendingMessages(id);
  }
}

void SenderQueueMonitor::sendToClient(uint32_t clientId,
                                      const ServerEventDTO &message) {
  std::lock_guard<std::mutex> lock(mutex);

  auto it = senderQueues.find(clientId);
  if (it == senderQueues.end())
    return;

  queuesPendingMessages[clientId].push(message);
}

void SenderQueueMonitor::sendToClient(uint32_t clientId,
                                      std::list<ServerEventDTO> &messages) {
  std::lock_guard<std::mutex> lock(mutex);

  auto it = senderQueues.find(clientId);
  if (it == senderQueues.end())
    return;

  for (auto &msg : messages) {
    queuesPendingMessages[clientId].push(std::move(msg));
  }
}

void SenderQueueMonitor::clearPendingMessages(uint32_t clientId) {
  auto &pending = queuesPendingMessages[clientId];
  auto *queue = senderQueues[clientId];

  while (!pending.empty()) {
    if (!queue->try_push(std::move(pending.front()))) {
      break;
    }

    pending.pop();
  }
}

void SenderQueueMonitor::markAsRegistered(uint32_t clientId) {
  std::lock_guard<std::mutex> lock(mutex);
  registeredClients.insert(clientId);
}

void SenderQueueMonitor::markAsUnregistered(uint32_t clientId) {
  std::lock_guard<std::mutex> lock(mutex);
  registeredClients.erase(clientId);
}

void SenderQueueMonitor::pushMessageToTheSenderQueues(
    const ServerEventDTO &message) {
  for (auto &[id, _] : senderQueues) {
    if (registeredClients.count(id)) {
      queuesPendingMessages[id].push(message);
    }
  }
}
