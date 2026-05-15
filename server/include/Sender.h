#ifndef SENDER_H
#define SENDER_H

#include "queue.h"
#include "socket.h"
#include "thread.h"
#include "SenderQueueMonitor.h"
#include "Command.h"
#include "CommunicationEnded.h"
#include <Protocol.h>


class Sender: public Thread {

private:
    SenderQueueMonitor& senderQueueMonitor;
    Socket& peer;
    Queue<Command>* senderQueue;
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
