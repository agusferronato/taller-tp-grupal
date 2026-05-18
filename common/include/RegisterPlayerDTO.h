#ifndef REGISTER_PLAYER_H
#define REGISTER_PLAYER_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <string>


class RegisterPlayerDTO : public CommandDTO {
private:
    std::string name;
 
public:

    explicit RegisterPlayerDTO(std::string name);
  
    const std::string& getName() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;

};


#endif