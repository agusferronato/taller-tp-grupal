#ifndef CLIENT_H
#define CLIENT_H

#include "RateLoop.h"
#include "ClientReceiver.h"
#include "ClientSender.h"
#include <socket.h>

class Client {


private:
    Socket socket;

public:

    Client(const char* hostname, const char* port);

    void run();

};


#endif /* CLIENT_H */