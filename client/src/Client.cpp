#include "Client.h"

Client::Client(const char *hostname, const char *port, const ClientData &data)
    : socket(Socket(hostname, port)), shutdownEvent(ShutdownEvent()),
      clientData(data) {}

void Client::run() {
  Queue<std::unique_ptr<CommandDTO>> sendingQueue, receptionQueue;
  ClientReceiver receiver(socket, receptionQueue, shutdownEvent);
  ClientSender sender(socket, sendingQueue, shutdownEvent);
  Gameloop gameloop(receptionQueue, sendingQueue, shutdownEvent, clientData);

  receiver.start();
  sender.start();
  gameloop.run();

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

  // gameloop.join();
  receiver.join();
  sender.join();
}
