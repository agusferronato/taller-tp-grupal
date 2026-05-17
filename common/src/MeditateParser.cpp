#include "MeditateParser.h"

void MeditateParser::getBytesToSend(std::vector<uint8_t>& bytes, CommandDTO& dto) {
    (void)dto;
    utils.appendToSend(static_cast<uint8_t>(CommandOpCode::MEDITATE), bytes);
}

std::unique_ptr<CommandDTO> MeditateParser::getDTO(Protocol& protocol) {
    (void)protocol;
    return std::make_unique<MeditateDTO>();
}
