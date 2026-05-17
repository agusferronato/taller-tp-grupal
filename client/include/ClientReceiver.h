#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>

#include "socket.h"
#include "queue.h"
#include "thread.h"
#include "CommandDTO.h"
#include "Protocol.h"
#include "CommunicationEnded.h"
#include "ShutdownEvent.h"

class ClientReceiver : public Thread {

private:
    Socket& socket;
    Queue<std::unique_ptr<CommandDTO>>& receptionQueue;
    ShutdownEvent& shutdownEvent;
    Protocol protocol;

public:
    ClientReceiver(Socket& socket,
                   Queue<std::unique_ptr<CommandDTO>>& receptionQueue,
                   ShutdownEvent& shutdownEvent);

    virtual void run() override;
};

#endif
