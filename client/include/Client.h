#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <sys/socket.h>

#include "socket.h"
#include "ShutdownEvent.h"
#include "queue.h"
#include "exceptions/closed_queue.h"
#include "CommandDTO.h"
#include "ClientReceiver.h"
#include "ClientSender.h"
#include "Gameloop.h"
#include "ClientData.h"

class Client {

private:
    Socket socket;
    ShutdownEvent shutdownEvent;
    ClientData clientData;

public:
    Client(const char* hostname, const char* port, const ClientData& data);

    void run();
};

#endif
