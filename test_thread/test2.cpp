
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <condition_variable>

// Worker receives a range (start + end iterator) with which it should
// work. It returns true if the computation should continue and false 
// if not.

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
    Worker(std::mutex& m) : m_(m), signal_(NONE), 
        state_(WAITING_FOR_NEXT_STEP) {
    }

    void start() {
      signal_ = NONE;
      state_ = WAITING_FOR_NEXT_STEP;
      //std::cout << "Starting thread: " << signal_ << "," << state_ << "\n";
      thread_ = std::thread(&Worker::thread_fun, this);
    }
    
    void step() {
      //std::cout << "Waiting for thread to finish: " << signal_ << "," << state_ << "\n";
      // wait until thread is waiting for next step 
      std::unique_lock<std::mutex> lk(m_);
      cv_.wait(lk, [this]{ return state_ != COMPUTING;});
      //std::cout << "Sending step: " << signal_ << "," << state_ << "\n";
      if (state_ == STOPPED) {
        //std::cout << "Stopped?" << signal_ << "," << state_ << "\n";
        return; // TODO
      }
      // then send message to start new step
      signal_ = STEP;
      state_ = COMPUTING;
      lk.unlock();
      cv_.notify_one();
    }

    void stop() {
      //std::cout << "Stop; Waiting for thread to finish: " << signal_ << "," << state_ << "\n";
      // wait until thread is waiting for next step 
      std::unique_lock<std::mutex> lk(m_);
      cv_.wait(lk, [this]{ return state_ != COMPUTING;});
      if (state_ != STOPPED) {
        signal_ = STOP;
        state_ = COMPUTING;
        lk.unlock();
        //std::cout << "Sending stop: " << signal_ << "," << state_ << "\n";
        cv_.notify_one();
      } else {
        lk.unlock();
      }
      // check if thread has stopped
      // if not stopped send message that is should stop
      //bool notify = false;
      //{
        //std::lock_guard<std::mutex> lock(m_);
        //std::cout << "Sending stop: " << signal_ << "," << state_ << "\n";
        //if (state_ != STOPPED) {
          //signal_ = STOP;
          //notify  = true;
        //}
      //}
      //if (notify) cv_.notify_one();
      // wait until thread has stopped and join thread
      //std::cout << "Waiting for join: " << signal_ << "," << state_ << "\n";
      if (thread_.joinable()) thread_.join();
    }

    void thread_fun() {
      // function doing the work; running in thread
      bool stop = false;
      while (!stop) {
        // wait for next step or stop
        std::unique_lock<std::mutex> lk(m_);
        //std::cout << "Waiting for step or stop: " << signal_ << "," << state_ << "\n";
        cv_.wait(lk, [this] { return signal_ != NONE;});
        //std::cout << "Received step or stop: " << signal_ << "," << state_ << "\n";
        if (signal_ == STOP) stop = true;
        signal_ = NONE;
        state_ = COMPUTING;
        // release lock during computation
        std::cout << "Starting computation: " << signal_ << "," << state_ << "\n";
        lk.unlock();
        // do computation
        if (!stop) compute();
        // signal back that we are finished
        lk.lock();
        std::cout << "Finished computation: " << signal_ << "," << state_ << "\n";
        state_ = stop ? STOPPED : WAITING_FOR_NEXT_STEP;
        lk.unlock(); 
        cv_.notify_all();
      }
    }

    void compute() {
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }

  private:
    std::thread thread_;
    std::condition_variable cv_;
    std::mutex& m_;
    Signal signal_;      // protected by m_
    WorkerState state_;  // protected by m_
};


int main(int argc, char* argv[]) {
  std::mutex m;
  //std::condition_variable cv;
  Worker worker1(m);
  Worker worker2(m);
  worker1.start();
  worker2.start();
  for (int i = 0; i < 5; ++i) {
    worker1.step();
    worker2.step();
  }
  worker1.stop();
  worker2.stop();
  
  return 0;
}
