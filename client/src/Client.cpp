#include "Client.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"

Client::Client(const char *hostname, const char *port, const ClientData &data)
    : socket(Socket(hostname, port)),
      clientData(data) {}

void Client::run() {
  Queue<ClientCommandDTO> sendingQueue;
  Queue<ServerEventDTO> receptionQueue;
  ClientReceiver receiver(socket, receptionQueue);
  ClientSender sender(socket, sendingQueue);

  receiver.start();
  sender.start();

  Gameloop gameloop(receptionQueue, sendingQueue, clientData);


  ShutdownReason shutdownReason = gameloop.run();

  if (shutdownReason == ShutdownReason::WindowClose) {
    socket.shutdown(SHUT_RDWR); 
    socket.close(); 
  }

  sendingQueue.close(); 
  receptionQueue.close(); 
    
  receiver.join();
  sender.join();
}
