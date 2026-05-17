#include "LoginPlayerParser.h"

void LoginPlayerParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    auto& loginDTO = dynamic_cast<LoginPlayerDTO&>(dto);
    utils.appendToSend(static_cast<uint8_t>(CommandOpCode::LOGIN_PLAYER), bytes);
    utils.appendToSend(loginDTO.getName(), bytes);
}

std::unique_ptr<CommandDTO> LoginPlayerParser::getDTO(Protocol& protocol) {
    std::string name;
    protocol.getStringData(name);
    return std::make_unique<LoginPlayerDTO>(std::move(name));
}
