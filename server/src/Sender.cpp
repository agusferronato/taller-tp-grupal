#include "Sender.h"
#include "RegisterAllParsers.h"

Sender::Sender(SenderQueueMonitor &senderQueueMonitor, Socket &peer)
    : senderQueueMonitor(senderQueueMonitor), peer(peer),
      senderQueue(senderQueueMonitor.getNewSenderQueue()), protocol(peer) {
  registerAllParsers(protocol);
}

void Sender::kill() {
  keepRunning = false;
  senderQueueMonitor.deleteSenderQueue(*senderQueue);
}

void Sender::run() {

  while (keepRunning) {

    try {

      auto serverMessage = senderQueue->pop();
      protocol.send(*serverMessage);

    } catch (const ClosedQueue &e) {
      delete senderQueue;
      break;

    } catch (const CommunicationEnded &e) {

      senderQueueMonitor.deleteSenderQueue(*this->senderQueue);
      delete senderQueue;
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
