#ifndef SENDER_QUEUE_MONITOR_H
#define SENDER_QUEUE_MONITOR_H

#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include "queue.h"
#include <Command.h>
#include "Constants.h"

class SenderQueueMonitor {

private:
    std::mutex mutex;
    std::list<Queue<Command>*> senderQueues;
    std::map<Queue<Command>*, std::queue<Command>> queuesPendingMessages;

public:
    SenderQueueMonitor() {}

    SenderQueueMonitor(const SenderQueueMonitor&) = delete;
    SenderQueueMonitor& operator=(const SenderQueueMonitor&) = delete;


    Queue<Command>* getNewSenderQueue();
    void deleteSenderQueue(Queue<Command>& senderQueue);

    void broadCast(std::list<Command>& pendingMessages);


private:
    void pushMessageToTheSenderQueues(Command& message);
    void clearPendingMessages(Queue<Command>& queue);
};


#endif
