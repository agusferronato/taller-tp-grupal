#include "RegisterPlayerDTO.h"

RegisterPlayerDTO::RegisterPlayerDTO(std::string name) : name(std::move(name)) {}

const std::string& RegisterPlayerDTO::getName() const { return name; }

uint8_t RegisterPlayerDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::RegisterPlayer);
}

std::unique_ptr<CommandDTO> RegisterPlayerDTO::clone() {
    return std::make_unique<RegisterPlayerDTO>(*this);
}
