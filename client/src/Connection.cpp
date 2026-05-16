#include "Connection.h"


Connection::Connection(
    Queue<Command>& receptionQueue,
    Queue<Command>& sendingQueue,
    Socket& socket,
    ShutdownEvent& shutdownEvent
) : receptionQueue(receptionQueue),
    sendingQueue(sendingQueue),
    socket(socket),
    receiver(socket, receptionQueue, shutdownEvent),
    sender(socket, sendingQueue, shutdownEvent),
    shutdownEvent(shutdownEvent) { }


void Connection::run() {
    receiver.start();
    sender.start();

    receiver.join();
    sender.join();
}


void Connection::close() {

    socket.shutdown(SHUT_RDWR);
    socket.close();
}
