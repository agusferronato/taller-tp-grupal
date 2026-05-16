#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "socket.h"
#include "queue.h"
#include "thread.h"
#include "Command.h"
#include "Protocol.h"
#include "CommunicationEnded.h"
#include "ShutdownEvent.h"


class ClientReceiver : public Thread {

private:
    Socket& socket;
    Queue<Command>& receptionQueue;
    ShutdownEvent& shutdownEvent;
    Protocol protocol;

public:

    ClientReceiver(Socket& socket, 
                   Queue<Command>& receptionQueue, 
                   ShutdownEvent& shutdownEvent);

    virtual void run() override;

};


#endif
