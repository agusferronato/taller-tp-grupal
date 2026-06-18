#include "Sender.h"
#include "protocol/RegisterAllParsers.h"

Sender::Sender(SenderQueueMonitor &senderQueueMonitor, Socket &peer,
               uint32_t clientId)
    : senderQueueMonitor(senderQueueMonitor), peer(peer), clientId(clientId),
      senderQueue(senderQueueMonitor.getNewSenderQueue(clientId)),
      protocol(peer) {
  registerAllParsers(protocol);
}

void Sender::kill() {
  keepRunning = false;
  senderQueueMonitor.deleteSenderQueue(clientId);
}

void Sender::run() {

  while (keepRunning) {

    try {

      auto serverMessage = senderQueue->pop();
      protocol.sendEvent(serverMessage);

    } catch (const ClosedQueue &e) {
      break;

    } catch (const CommunicationEnded &e) {

      senderQueueMonitor.deleteSenderQueue(this->clientId);
      break;

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
