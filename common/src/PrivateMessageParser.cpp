#include "PrivateMessageParser.h"

void PrivateMessageParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& pmDTO = dynamic_cast<PrivateMessageDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(CommandOpCode::PRIVATE_MESSAGE), bytes);
    utils.appendToSend(pmDTO.getTarget(), bytes);
    utils.appendToSend(pmDTO.getMessage(), bytes);
}

std::unique_ptr<CommandDTO> PrivateMessageParser::getDTO(Protocol& protocol) {
    std::string target;
    std::string message;
    protocol.getStringData(target);
    protocol.getStringData(message);
    return std::make_unique<PrivateMessageDTO>(std::move(target), std::move(message));
}
