#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include "socket.h"
#include "Command.h"
#include "queue.h"
#include "Protocol.h"
#include "CommunicationEnded.h"

class ClientSender : public Thread {


private:
    Socket& socket;
    Queue<Command> &sendingQueue;
    Protocol protocol;
    bool isRunning = true;


public:

    ClientSender(Socket& socket, Queue<Command> &sendingQueue);

    virtual void run() override;

};


#endif