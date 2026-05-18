#include "Receiver.h"
#include "RegisterAllParsers.h"

Receiver::Receiver(Socket& peer, Queue<std::unique_ptr<CommandDTO>>& gameloopQueue):
        peer(peer), gameloopQueue(gameloopQueue), protocol(peer) {
    registerAllParsers(protocol);
}

void Receiver::kill() { keepRunning = false; }

void Receiver::run() {

    while (keepRunning) {

        try {
            auto command = protocol.receive();
            gameloopQueue.push(std::move(command));

        } catch (const CommunicationEnded& e) {
            break;

        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
