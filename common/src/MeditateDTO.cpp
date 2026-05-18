#include "MeditateDTO.h"

MeditateDTO::MeditateDTO() {}

uint8_t MeditateDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::MEDITATE);
}

std::unique_ptr<CommandDTO> MeditateDTO::clone() {
    return std::make_unique<MeditateDTO>(*this);
}
