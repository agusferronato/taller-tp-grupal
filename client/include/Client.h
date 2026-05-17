#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include "socket.h"
#include "ShutdownEvent.h"
#include "queue.h"
#include "closed_queue.h"
#include "Command.h"
#include "ClientReceiver.h"
#include "ClientSender.h"
#include "Gameloop.h"


class Client {


private:
    Socket socket;
    ShutdownEvent shutdownEvent;

public:

    Client(const char* hostname, const char* port);

    void run();


};


#endif /* CLIENT_H */