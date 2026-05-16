#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include "socket.h"
#include "Command.h"
#include "queue.h"
#include "Protocol.h"
#include "CommunicationEnded.h"
#include "thread.h"
#include "ShutdownEvent.h"


class ClientSender : public Thread {


private:
    Socket& socket;
    Queue<Command>& sendingQueue;
    ShutdownEvent& shutdownEvent;
    Protocol protocol;

public:

    ClientSender(Socket& socket, 
                 Queue<Command>& sendingQueue, 
                 ShutdownEvent& shutdownEvent);

    virtual void run() override;

};


#endif
