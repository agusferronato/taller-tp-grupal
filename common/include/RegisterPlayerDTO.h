#ifndef REGISTER_PLAYER_H
#define REGISTER_PLAYER_H

#include "CommandDTO.h"
#include "Protocol.h"
#include <string>
#include <memory>
#include "protocol_codes.h"


class RegisterPlayerDTO : public CommandDTO {
private:
    std::string name;
 
public:

    explicit RegisterPlayerDTO(std::string name) : name(std::move(name)) {}
 
    const std::string& getName() const { return name; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::RegisterPlayer);
    }

};


#endif