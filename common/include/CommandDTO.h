#ifndef COMMAND_DTO_H
#define COMMAND_DTO_H

#include <cstdint>
#include <memory>


class CommandDTO {
    
public:
    virtual uint8_t getCode() = 0;
    virtual std::unique_ptr<CommandDTO> clone() = 0;
    virtual ~CommandDTO() = default;

};



#endif
