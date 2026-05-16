#ifndef CLIENT_H
#define CLIENT_H

#include <socket.h>
#include <sys/socket.h>
#include "ShutdownEvent.h"
#include "GameLoop.h"
#include "RateLoop.h"
#include "Connection.h"

class Client {


private:
    Socket socket;

public:

    Client(const char* hostname, const char* port);

    void run();
};


#endif /* CLIENT_H */
