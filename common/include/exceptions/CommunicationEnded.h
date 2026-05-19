#ifndef COMMUNICATION_ENDED_H
#define COMMUNICATION_ENDED_H

#include <cstdarg>
#include <cstdio>
#include <exception>


#define BUF_LEN 256

class CommunicationEnded: std::exception {

private:
    char msg[BUF_LEN];

public:
    CommunicationEnded(const char* format, ...);
    virtual const char* what() const noexcept;
};


#endif
