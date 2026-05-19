#ifndef CONSTANT_RATELOOP_H
#define CONSTANT_RATELOOP_H

#include "Constants.h"
#include "thread.h"
#include <chrono>
#include <cmath>
#include <thread>

class ConstantRateLoop {
public:
  explicit ConstantRateLoop(int framesPerSecond);
  virtual ~ConstantRateLoop() = default;

  virtual void updateTimer(unsigned int &it);

private:
  int rateInMiliseconds;
  std::chrono::time_point<std::chrono::steady_clock> t0;
};

#endif
