#include "Protocol.h"

Protocol::Protocol(Socket& socket) : socket(socket) {}

void Protocol::registerParser(uint8_t code, std::unique_ptr<CommandParser> parser) {
    parsers[code] = std::move(parser);
}

void Protocol::send(CommandDTO& command) {
    std::vector<uint8_t> bytes;
    uint8_t code = command.getCode();
    auto it = parsers.find(code);

    it->second->getBytesToSend(bytes, command);
    if (!bytes.empty())
        socket.sendall(bytes.data(), bytes.size());
}

std::unique_ptr<CommandDTO> Protocol::receive() {
    uint8_t code = utils.receive_uint8(socket);
    auto it = parsers.find(code);

    return it->second->getDTO(*this);
}

void Protocol::getStringData(std::string& str) {
    utils.recv_string(socket, str);
}

uint8_t Protocol::getUint8() {
    return utils.receive_uint8(socket);
}

uint16_t Protocol::getUint16() {
    return utils.receive_uint16(socket);
}

int16_t Protocol::getInt16() {
    return utils.receive_int16(socket);
}

uint32_t Protocol::getUint32() {
    return utils.receive_uint32(socket);
}
