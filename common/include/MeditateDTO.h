#ifndef MEDITATE_DTO_H
#define MEDITATE_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"

class MeditateDTO : public CommandDTO {
public:
    MeditateDTO() = default;

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::MEDITATE);
    }
};

#endif
