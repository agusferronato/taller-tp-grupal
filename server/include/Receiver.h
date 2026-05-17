#ifndef RECEIVER_H
#define RECEIVER_H

#include <string>
#include <utility>
#include <memory>

#include "CommunicationEnded.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"
#include "CommandDTO.h"
#include "Protocol.h"

class Receiver: public Thread {

private:
    Socket& peer;
    Queue<std::unique_ptr<CommandDTO>>& gameloopQueue;
    Protocol protocol;
    bool keepRunning = true;

public:
    Receiver(Socket& peer, Queue<std::unique_ptr<CommandDTO>>& gameloopQueue);

    Receiver(const Receiver&) = delete;
    Receiver& operator=(const Receiver&) = delete;

    void kill();

    virtual void run() override;
};

#endif
