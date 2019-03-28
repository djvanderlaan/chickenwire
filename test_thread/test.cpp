
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <condition_variable>


class ThreadFun {
  public:
    ThreadFun(int id, int counter): id_(id), counter_(counter) {
    }

    void thread_start() {
      ready_ = false;
      finished_ = false;
      stop_ = false;

      std::thread worker(&ThreadFun::step, this);
      for (int i = 0; i < 100; ++i) {
        // Notify worker thread that it can start its computation
        {
          std::lock_guard<std::mutex> lk(m_);
          ready_ = true;
          std::cout << "Notifying worker\n";
        }
        cv_.notify_one();
        // Wait for worker thread to finish
        {
          std::cout << "Waiting for worker\n";
          std::unique_lock<std::mutex> lk(m_);
          cv_.wait(lk, [this]{ return finished_;});
          finished_ = false;
          if (stop_) break;
          std::cout << "Counter = " << counter_ << "\n";
        }
      }
      std::cout << "Joining worker\n";

      worker.join();

      std::cout << counter_ << "\n";
    }

  protected: 
    void step() {
      bool cont = true;
      while (cont) {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this]{ return ready_;});
        ++counter_;
        stop_ = counter_ > 50;
        if (stop_) cont = false;
        finished_ = true;
        ready_ = false;
        lk.unlock();
        cv_.notify_one();
      }
    }


  private:
    int id_;
    int counter_;

    // thread control
    std::mutex m_;
    std::condition_variable cv_;
    bool ready_;
    bool finished_;
    bool stop_;
};


int main(int argc, char* argv[]) {
  ThreadFun fun(1, 0);
  fun.thread_start();
  return 0;
}
