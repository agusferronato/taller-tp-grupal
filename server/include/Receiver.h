#ifndef RECEIVER_H
#define RECEIVER_H


#include <string>
#include <utility>

#include "CommunicationEnded.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"
#include "Command.h"
#include <Protocol.h>


class Receiver: public Thread {

private:
    Socket& peer;
    Queue<Command>& gameloopQueue;
    Protocol protocol;
    bool keepRunning = true;

public:
    Receiver(Socket& peer, Queue<Command>& gameloopQueue);

    Receiver(const Receiver&) = delete;
    Receiver& operator=(const Receiver&) = delete;

    void kill();

    virtual void run() override;
};


#endif
