#include "ClientEncoder.h"


ClientEncoder::ClientEncoder(ClinetProtocol& protocol) 
    : protocol(protocol) { }


void ClientEncoder::encode(const Command &cmd)
{
    std::vector<uint8_t> bytesToSend = cmd.getBytes();
    protocol.send(bytesToSend);
} 

