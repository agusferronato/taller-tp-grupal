#ifndef SENDER_H
#define SENDER_H

#include <memory>

#include "queue.h"
#include "socket.h"
#include "thread.h"
#include "SenderQueueMonitor.h"
#include "CommandDTO.h"
#include "CommunicationEnded.h"
#include "Protocol.h"

class Sender: public Thread {

private:
    SenderQueueMonitor& senderQueueMonitor;
    Socket& peer;
    Queue<std::unique_ptr<CommandDTO>>* senderQueue;
    Protocol protocol;
    bool keepRunning = true;

public:
    Sender(SenderQueueMonitor& senderQueueMonitor, Socket& peer);

    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    void kill();
    virtual void run() override;
};

#endif
