#include "ShutdownEvent.h"


bool ShutdownEvent::finished() {
    return occurred;
}

void ShutdownEvent::put(ShutdownReason reason) {
    std::lock_guard<std::mutex> lck(mtx);
    if (occurred)
        return;
    occurred = true;
    this->reason = reason;
    notification.notify_all();
}

ShutdownReason ShutdownEvent::getReason() {
    return reason;
}

void ShutdownEvent::wait() {
    std::unique_lock<std::mutex> lck(mtx);
    notification.wait(lck, [this]{ return occurred.load(); });
}
