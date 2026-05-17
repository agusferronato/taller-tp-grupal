#ifndef EXIT_DTO_H
#define EXIT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"

class ExitDTO : public CommandDTO {
public:
    ExitDTO() = default;

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::EXIT);
    }
};

#endif
