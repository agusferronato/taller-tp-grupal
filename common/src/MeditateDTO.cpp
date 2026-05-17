#include "MeditateDTO.h"

MeditateDTO::MeditateDTO() {}

uint8_t MeditateDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::MEDITATE);
}
