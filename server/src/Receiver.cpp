#include "Receiver.h"


Receiver::Receiver(Socket& peer, Queue<Command>& gameloopQueue):
        peer(peer), gameloopQueue(gameloopQueue), protocol(peer) { }

void Receiver::kill() { keepRunning = false; }


void Receiver::run() {

    while (keepRunning) {

        try {
            Command command = protocol.receive();
            gameloopQueue.push(command);

        } catch (const CommunicationEnded& e) {
            break;

        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
