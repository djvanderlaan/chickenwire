
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <condition_variable>

typedef enum {
  NONE,
  STEP,
  STOP
} Signal;

typedef enum {
  COMPUTING,
  WAITING_FOR_NEXT_STEP,
  STOPPED
} WorkerState;

class Worker {
  public:
    Worker(std::condition_variable& cv, std::mutex& m) : cv_(cv), m_(m), signal_(NONE), 
        state_(WAITING_FOR_NEXT_STEP) {
    }

    void start() {
      signal_ = NONE;
      state_ = WAITING_FOR_NEXT_STEP;
      thread_ = std::thread(&Worker::thread_fun, this);
    }
    
    void step() {
    }

    void stop() {
      // if thread is not yet stopped signal that it should stop and
      // wait until it is stopped.
    }

    void thread_fun() {
      // function doing the work; running in thread
      bool stop = false;
      while (!stop) {
        // wait for next step or stop
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this] { return signal_ != NONE;});
        if (signal_ == STOP) stop = true;
        signal_ = NONE;
        state_ = COMPUTING;
        // release lock during computation
        lk.unlock();
        // do computation
        if (!stop) compute();
        // signal back that we are finished
        lk.lock();
        state_ = stop ? STOPPED : WAITING_FOR_NEXT_STEP;
        lk.unlock(); 
        cv_.notify_all();
      }
    }

    void compute() {
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }

  private:
    std::thread thread_;
    std::condition_variable& cv_;
    std::mutex& m_;
    Signal signal_;      // protected by m_
    WorkerState state_;  // protected by m_
};


int main(int argc, char* argv[]) {
  return 0;
}
