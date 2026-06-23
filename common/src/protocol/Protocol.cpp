#include "protocol/Protocol.h"

#include "CommunicationEnded.h"
#include "ProtocolError.h"
#include "protocol/ClientCommandCodes.h"
#include "protocol/ServerEventCodes.h"

Protocol::Protocol(Socket &socket) : socket(socket) {}

void Protocol::registerCommandParser(
    uint8_t code, std::unique_ptr<ClientCommandParser> parser) {
  commandParsers[code] = std::move(parser);
}

void Protocol::registerEventParser(uint8_t code,
                                   std::unique_ptr<ServerEventParser> parser) {
  eventParsers[code] = std::move(parser);
}

void Protocol::sendCommand(const ClientCommandDTO &command) {
  std::vector<uint8_t> bytes;

  uint8_t code = static_cast<uint8_t>(getCode(command));

  auto it = commandParsers.find(code);
  if (it == commandParsers.end()) {
    throw ProtocolError("Unknown command opcode to send: " +
                        std::to_string(code));
  }

  it->second->serialize(bytes, command);

  if (!bytes.empty()) {
    if (socket.is_stream_send_closed()) {
      throw CommunicationEnded("Connection closed by peer");
    }

    socket.sendall(bytes.data(), bytes.size());
  }
}

ClientCommandDTO Protocol::receiveCommand() {
  uint8_t code = utils.receiveUint8(socket);

  if (socket.is_stream_recv_closed()) {
    throw CommunicationEnded("Connection closed by peer");
  }

  auto it = commandParsers.find(code);
  if (it == commandParsers.end()) {
    throw ProtocolError("Unknown command opcode received: " +
                        std::to_string(code));
  }

  return it->second->deserialize(*this);
}

void Protocol::sendEvent(const ServerEventDTO &event) {
  std::vector<uint8_t> bytes;

  uint8_t code = static_cast<uint8_t>(getCode(event));

  auto it = eventParsers.find(code);
  if (it == eventParsers.end()) {
    throw ProtocolError("Unknown event opcode to send: " +
                        std::to_string(code));
  }

  it->second->serialize(bytes, event);

  if (!bytes.empty()) {
    if (socket.is_stream_send_closed()) {
      throw CommunicationEnded("Connection closed by peer");
    }

    socket.sendall(bytes.data(), bytes.size());
  }
}

ServerEventDTO Protocol::receiveEvent() {
  uint8_t code = utils.receiveUint8(socket);

  if (socket.is_stream_recv_closed()) {
    throw CommunicationEnded("Connection closed by peer");
  }

  auto it = eventParsers.find(code);
  if (it == eventParsers.end()) {
    throw ProtocolError("Unknown event opcode to receive: " +
                        std::to_string(code));
  }

  return it->second->deserialize(*this);
}

std::string Protocol::getStringData() {
  std::string str;
  utils.receiveString(socket, str);
  return str;
}

uint8_t Protocol::getUint8() { return utils.receiveUint8(socket); }

uint16_t Protocol::getUint16() { return utils.receiveUint16(socket); }

int16_t Protocol::getInt16() { return utils.receiveInt16(socket); }

uint32_t Protocol::getUint32() { return utils.receiveInt32(socket); }
