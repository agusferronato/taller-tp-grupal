#ifndef COMMAND_DTO_H
#define COMMAND_DTO_H

#include <cstdint>


class CommandDTO {
    
public:
    virtual uint8_t getCode() = 0;
    virtual ~CommandDTO() = default;

};



#endif
