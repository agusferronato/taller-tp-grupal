#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "Command.h"

class Protocol {

private:

    Socket& socket;


public:

    Protocol (Socket& socket);

    Command receive();
    void send(Command& command); 
    
};


#endif /* PROTOCOL_H */