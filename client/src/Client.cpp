#include "Client.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"

Client::Client(const char *hostname, const char *port, const ClientData &data)
    : socket(Socket(hostname, port)), shutdownEvent(ShutdownEvent()),
      clientData(data) {}

void Client::run() {
  Queue<ClientCommandDTO> sendingQueue;
  Queue<ServerEventDTO> receptionQueue;
  ClientReceiver receiver(socket, receptionQueue, shutdownEvent);
  ClientSender sender(socket, sendingQueue, shutdownEvent);

  receiver.start();
  sender.start();

  try {
    Gameloop gameloop(receptionQueue, sendingQueue, shutdownEvent, clientData);
    gameloop.run();
  } catch (...) {
    shutdownEvent.put(ShutdownReason::ConnectionClosed);
    sendingQueue.close();
    receptionQueue.close();
    receiver.join();
    sender.join();
    throw;
  }

  shutdownEvent.wait();

  switch (shutdownEvent.getReason()) {

  case ShutdownReason::SDLQuit:
    socket.shutdown(SHUT_RDWR);
    socket.close();
    [[fallthrough]];

  case ShutdownReason::ConnectionClosed:
    sendingQueue.close();
    receptionQueue.close();

  default:
    break;
  }

  receiver.join();
  sender.join();
}
