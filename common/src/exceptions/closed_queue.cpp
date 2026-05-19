#include "closed_queue.h"

ClosedQueue::ClosedQueue() : std::runtime_error("The queue is closed") {}
