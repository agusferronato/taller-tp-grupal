#ifndef EXIT_DTO_H
#define EXIT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"

class ExitDTO : public CommandDTO {
public:
    ExitDTO();

    uint8_t getCode() override;
};

#endif
