#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <sys/socket.h>

#include "ClientData.h"
#include "ClientReceiver.h"
#include "ClientSender.h"
#include "CommandDTO.h"
#include "Gameloop.h"
#include "ShutdownEvent.h"
#include "exceptions/closed_queue.h"
#include "queue.h"
#include "socket.h"

class Client {

private:
  Socket socket;
  ShutdownEvent shutdownEvent;
  ClientData clientData;

public:
  Client(const char *hostname, const char *port, const ClientData &data);

  void run();
};

#endif
