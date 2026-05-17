#include "Acceptor.h"

Acceptor::Acceptor(Socket& socket, Queue<std::unique_ptr<CommandDTO>>& gameloopQueue,
                   SenderQueueMonitor& senderQueueMonitor):
        acceptor(socket), gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}


void Acceptor::run() {

    std::vector<ClientConnection*> connections;

    while (keepRunning) {

        try {
            Socket peer = acceptor.accept();

            ClientConnection* connection =
                    new ClientConnection(std::move(peer), gameloopQueue, senderQueueMonitor);

            reap(connections);
            connections.push_back(connection);
            connection->start();

        } catch (...) {
            break;
        }
    }
    clear(connections);
}

void Acceptor::kill() {
    keepRunning = false;
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

void Acceptor::reap(std::vector<ClientConnection*>& connections) {

    auto shouldReap = [](ClientConnection& client) {
        bool isDead = client.isDead();
        if (isDead) {
            client.join();
            delete &client;
        }
        return isDead;
    };

    auto it = connections.begin();
    while (it != connections.end()) {
        if (shouldReap(**it)) {
            it = connections.erase(it);
        } else {
            ++it;
        }
    }
}

void Acceptor::clear(std::vector<ClientConnection*>& connections) {

    for (auto* client: connections) {
        client->kill();
        client->join();
        delete client;
    }
    connections.clear();
}
