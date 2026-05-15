#include "SenderQueueMonitor.h"


Queue<Command>* SenderQueueMonitor::getNewSenderQueue() {
    std::lock_guard<std::mutex> lock(mutex);

    Queue<Command>* senderQueue = new Queue<Command>(SENDER_QUEUE_SIZE);
    std::queue<Command> pendingMessages;

    senderQueues.push_back(senderQueue);

    queuesPendingMessages.insert({senderQueue, std::move(pendingMessages)});

    return senderQueue;
}

void SenderQueueMonitor::deleteSenderQueue(Queue<Command>& senderQueue) {
    std::lock_guard<std::mutex> lock(mutex);

    senderQueue.close();
    senderQueues.remove(&senderQueue);
    queuesPendingMessages.erase(&senderQueue);
}


void SenderQueueMonitor::broadCast(std::list<Command>& messagesToSend) {

    std::lock_guard<std::mutex> lock(mutex);

    for (Command& message: messagesToSend) pushMessageToTheSenderQueues(message);

    for (Queue<Command>* queue: senderQueues) clearPendingMessages(*queue);
}


void SenderQueueMonitor::clearPendingMessages(Queue<Command>& queue) {

    while (!queuesPendingMessages[&queue].empty()) {

        Command& pendingMessage = queuesPendingMessages[&queue].front();
        if (!queue.try_push(pendingMessage))
            break;

        queuesPendingMessages[&queue].pop();
    }
}


void SenderQueueMonitor::pushMessageToTheSenderQueues(Command& message) {

    for (Queue<Command>* queue: senderQueues) queuesPendingMessages[queue].push(message);
}
