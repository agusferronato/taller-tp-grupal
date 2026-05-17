#include "../include/client_encoder.h"
#include "../../common/include/exceptions/protocol_error.h"
#include "../../common/include/protocol_codes.h"
#include "../include/client_protocol.h"

void ClientEncoder::encode(ClientProtocol &protocol, const CommandDTO &cmd) {

  if (std::holds_alternative<RegisterPlayerDTO>(cmd)) {
    send(protocol, std::get<RegisterPlayerDTO>(cmd));

  } else if (std::holds_alternative<PrivateMessageDTO>(cmd)) {
    send(protocol, std::get<PrivateMessageDTO>(cmd));

  } else {
    throw ProtocolError("Unable to send command: Unknown client command");
  }
}

void ClientEncoder::send(ClientProtocol &protocol,
                         const RegisterPlayerDTO &cmd) {
  protocol.send_uint8(
      static_cast<uint8_t>(protocol::ClientOpcode::REGISTER_PLAYER));
  protocol.send_string(cmd.name);
}

void ClientEncoder::send(ClientProtocol &protocol,
                         const PrivateMessageDTO &cmd) {
  protocol.send_uint8(
      static_cast<uint8_t>(protocol::ClientOpcode::PRIVATE_MESSAGE));
  protocol.send_string(cmd.target);
  protocol.send_string(cmd.message);
}
