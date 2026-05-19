#ifndef WINDOW_CLOSED_H
#define WINDOW_CLOSED_H

#include <cstdarg>
#include <cstdio>
#include <exception>


#define BUF_LEN 256

class WindowClosed: std::exception {

private:
    char msg[BUF_LEN];

public:
    WindowClosed(const char* format, ...);
    virtual const char* what() const noexcept;
};


#endif
