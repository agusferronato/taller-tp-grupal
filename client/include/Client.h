#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <sys/socket.h>

#include "ClientData.h"
#include "ClientReceiver.h"
#include "ClientSender.h"
#include "ClosedQueue.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "Gameloop.h"
#include "Queue.h"
#include "Socket.h"

class Client {

private:
  Socket socket;
  ClientData clientData;

public:
  Client(const char *hostname, const char *port, const ClientData &data);

  void run();
};

#endif
