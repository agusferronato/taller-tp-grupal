#ifndef CLOSED_QUEUE_H_
#define CLOSED_QUEUE_H_

#include <stdexcept>

struct ClosedQueue : public std::runtime_error {
  ClosedQueue() : std::runtime_error("The queue is closed") {}
};

#endif // CLOSED_QUEUE_H_
