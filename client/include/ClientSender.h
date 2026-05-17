#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <memory>

#include "socket.h"
#include "CommandDTO.h"
#include "queue.h"
#include "Protocol.h"
#include "CommunicationEnded.h"
#include "thread.h"
#include "ShutdownEvent.h"

class ClientSender : public Thread {

private:
    Socket& socket;
    Queue<std::unique_ptr<CommandDTO>>& sendingQueue;
    ShutdownEvent& shutdownEvent;
    Protocol protocol;

public:
    ClientSender(Socket& socket,
                 Queue<std::unique_ptr<CommandDTO>>& sendingQueue,
                 ShutdownEvent& shutdownEvent);

    virtual void run() override;
};

#endif
