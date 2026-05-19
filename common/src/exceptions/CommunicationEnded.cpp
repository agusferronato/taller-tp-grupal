#include "CommunicationEnded.h"

CommunicationEnded::CommunicationEnded(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(msg, BUF_LEN, format, args);
    va_end(args);
}

const char* CommunicationEnded::what() const noexcept { return msg; }
