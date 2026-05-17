#include "PrivateMessageDTO.h"

PrivateMessageDTO::PrivateMessageDTO(std::string target, std::string message)
    : target(std::move(target)), message(std::move(message)) {}

const std::string& PrivateMessageDTO::getTarget() const { return target; }

const std::string& PrivateMessageDTO::getMessage() const { return message; }

uint8_t PrivateMessageDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::PRIVATE_MESSAGE);
}
