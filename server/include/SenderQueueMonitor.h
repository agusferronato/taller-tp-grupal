#ifndef SENDER_QUEUE_MONITOR_H
#define SENDER_QUEUE_MONITOR_H

#include <cstdint>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <unordered_set>

#include "Constants.h"
#include "DTO/Events/EventDTO.h"
#include "Queue.h"

class SenderQueueMonitor {
private:
  std::mutex mutex;
  std::map<uint32_t, Queue<ServerEventDTO> *> senderQueues;
  std::map<uint32_t, std::queue<ServerEventDTO>> queuesPendingMessages;
  std::unordered_set<uint32_t> registeredClients;

public:
  SenderQueueMonitor();

  SenderQueueMonitor(const SenderQueueMonitor &) = delete;
  SenderQueueMonitor &operator=(const SenderQueueMonitor &) = delete;

  Queue<ServerEventDTO> *getNewSenderQueue(uint32_t clientId);
  void deleteSenderQueue(uint32_t clientId);

  void broadCast(std::list<ServerEventDTO> &pendingMessages);
  void sendToClient(uint32_t clientId, const ServerEventDTO &message);
  void sendToClient(uint32_t clientId, std::list<ServerEventDTO> &messages);

  void markAsRegistered(uint32_t clientId);
  void markAsUnregistered(uint32_t clientId);

private:
  void pushMessageToTheSenderQueues(const ServerEventDTO &message);
  void clearPendingMessages(uint32_t clientId);
};

#endif
