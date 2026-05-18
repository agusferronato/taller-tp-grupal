#include "LoginPlayerDTO.h"

LoginPlayerDTO::LoginPlayerDTO(std::string name) : name(std::move(name)) {}

const std::string& LoginPlayerDTO::getName() const { return name; }

uint8_t LoginPlayerDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::LOGIN_PLAYER);
}

std::unique_ptr<CommandDTO> LoginPlayerDTO::clone() {
    return std::make_unique<LoginPlayerDTO>(*this);
}
