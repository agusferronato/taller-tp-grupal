#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <algorithm>
#include <utility>
#include <vector>

#include "socket.h"
#include "thread.h"
#include "ClientConnection.h"
#include "SenderQueueMonitor.h"
#include "Command.h"
#include <sys/socket.h>


class Acceptor: public Thread {


private:
    Socket& acceptor;
    Queue<Command>& gameloopQueue;
    SenderQueueMonitor& senderQueueMonitor;
    bool keepRunning = true;

public:
    Acceptor(Socket& socket, Queue<Command>& gameloopQueue,
             SenderQueueMonitor& senderQueueMonitor);

    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;

    virtual void run() override;

    void kill();

    void reap(std::vector<ClientConnection*>& connections);
    void clear(std::vector<ClientConnection*>& connections);
};


#endif