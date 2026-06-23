#ifndef THREAD_H_
#define THREAD_H_

#include <atomic>
#include <thread>

class Runnable {
public:
  virtual void start() = 0;
  virtual void join() = 0;
  virtual void stop() = 0;
  virtual bool is_alive() const = 0;

  virtual ~Runnable() {}
};

class Thread : public Runnable {
private:
  std::thread thread;

  std::atomic<bool> _keep_running;
  std::atomic<bool> _is_alive;

protected:
  bool should_keep_running() const;

public:
  Thread();

  void start() override;
  void join() override;

  void main();

  void stop() override;

  bool is_alive() const override;

  virtual void run() = 0;
  virtual ~Thread();

  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;

  Thread(Thread &&other) = delete;
  Thread &operator=(Thread &&other) = delete;
};

#endif
