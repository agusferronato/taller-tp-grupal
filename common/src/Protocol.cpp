#include "Protocol.h"

Protocol::Protocol(Socket &socket) : socket(socket)
{
}

Command Protocol::receive()
{
    return Command();
}

void Protocol::send(Command &command)
{
    (void)command;
}
