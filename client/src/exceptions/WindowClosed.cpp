#include "WindowClosed.h"

WindowClosed::WindowClosed(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf(msg, BUF_LEN, format, args);
  va_end(args);
}

const char *WindowClosed::what() const noexcept { return msg; }
