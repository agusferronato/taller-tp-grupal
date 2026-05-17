#ifndef LOGIN_PLAYER_DTO_H
#define LOGIN_PLAYER_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <string>

class LoginPlayerDTO : public CommandDTO {
private:
    std::string name;

public:
    explicit LoginPlayerDTO(std::string name) : name(std::move(name)) {}

    const std::string& getName() const { return name; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::LOGIN_PLAYER);
    }
};

#endif
