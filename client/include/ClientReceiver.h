#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "socket.h"
#include "queue.h"
#include "thread.h"
#include "Command.h"
#include "Protocol.h"
#include "CommunicationEnded.h"


class ClientReceiver : public Thread {

private:
    Socket& socket;
    Queue<Command> &receptionQueue;
    Protocol protocol;
    bool isRunning = true;


public:

    ClientReceiver(Socket& socket, Queue<Command> &receptionQueue);

    virtual void run() override;

};


#endif