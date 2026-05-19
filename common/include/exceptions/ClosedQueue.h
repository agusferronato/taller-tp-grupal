#ifndef CLOSED_QUEUE_H_
#define CLOSED_QUEUE_H_

#include <stdexcept>

struct ClosedQueue : public std::runtime_error {
  ClosedQueue();
};

#endif // CLOSED_QUEUE_H_
