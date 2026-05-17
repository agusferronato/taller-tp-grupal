#include "ExitDTO.h"

ExitDTO::ExitDTO() {}

uint8_t ExitDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::EXIT);
}
