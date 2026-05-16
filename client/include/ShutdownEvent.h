#ifndef SHUTDOWN_EVENT_H
#define SHUTDOWN_EVENT_H


#include <atomic>
#include <condition_variable>
#include <mutex>
#include "ShutdownReason.h"

class ShutdownEvent {


private:

    std::atomic<bool> occurred = false;
    std::atomic<ShutdownReason> reason;
    std::mutex mtx;
    std::condition_variable notification;


public:

    bool finished();
    void put(ShutdownReason reason);
    ShutdownReason getReason();
    void wait();

};


#endif
