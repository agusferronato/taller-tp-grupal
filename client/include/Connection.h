#ifndef CONNECTION_H
#define CONNECTION_H

#include "ClientReceiver.h"
#include "ClientSender.h"
#include "thread.h"
#include "ShutdownEvent.h"
#include "queue.h"
#include <sys/socket.h>

class Connection : public Thread {

private:
    Queue<Command>& receptionQueue;
    Queue<Command>& sendingQueue;

    Socket& socket;

    ClientReceiver receiver;
    ClientSender sender;

    ShutdownEvent& shutdownEvent;

public:

    Connection(
        Queue<Command>& receptionQueue,
        Queue<Command>& sendingQueue,
        Socket& socket,
        ShutdownEvent& shutdownEvent
    );

    virtual void run() override;

    void close();

};


#endif
