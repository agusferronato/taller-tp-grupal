#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <algorithm>
#include <utility>
#include <vector>
#include <memory>

#include "socket.h"
#include "thread.h"
#include "ClientConnection.h"
#include "SenderQueueMonitor.h"
#include "CommandDTO.h"
#include <sys/socket.h>

class Acceptor: public Thread {

private:
    Socket& acceptor;
    Queue<std::unique_ptr<CommandDTO>>& gameloopQueue;
    SenderQueueMonitor& senderQueueMonitor;
    bool keepRunning = true;

public:
    Acceptor(Socket& socket, Queue<std::unique_ptr<CommandDTO>>& gameloopQueue,
             SenderQueueMonitor& senderQueueMonitor);

    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;

    virtual void run() override;

    void kill();

    void reap(std::vector<ClientConnection*>& connections);
    void clear(std::vector<ClientConnection*>& connections);
};

#endif
