#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <utility>
#include <memory>

#include "socket.h"
#include "thread.h"
#include <sys/socket.h>
#include "Receiver.h"
#include "Sender.h"
#include "SenderQueueMonitor.h"
#include "CommandDTO.h"

class ClientConnection: public Thread {

private:
    Socket peer;
    Receiver receiver;
    Sender sender;

public:
    ClientConnection(Socket&& peer, Queue<std::unique_ptr<CommandDTO>>& gameloopQueue,
                     SenderQueueMonitor& senderQueueMonitor);

    ClientConnection(const ClientConnection&) = delete;
    ClientConnection& operator=(const ClientConnection&) = delete;

    void kill();

    bool isDead();

    virtual void run() override;
};

#endif
