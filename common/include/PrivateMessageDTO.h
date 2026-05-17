#ifndef PRIVATE_MESSAGE_DTO_H
#define PRIVATE_MESSAGE_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <string>

class PrivateMessageDTO : public CommandDTO {
private:
    std::string target;
    std::string message;

public:
    PrivateMessageDTO(std::string target, std::string message)
        : target(std::move(target)), message(std::move(message)) {}

    const std::string& getTarget() const { return target; }
    const std::string& getMessage() const { return message; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::PRIVATE_MESSAGE);
    }
};

#endif
