#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <algorithm>
#include <condition_variable>

template<typename T>
class Stepper {
  public:
    Stepper(T& computation) : computation_(computation) {
    }

    void run(unsigned int nworkers) {
      std::vector<std::thread> workers;
      current_step.clear();
      stop_iteration.clear();
      step = 0;
      for (int i = 0; i < nworkers; ++i) {
        current_step.push_back(0);
        stop_iteration.push_back(false);
        workers.push_back(std::thread(&Stepper::worker_function, this, i, nworkers));
      }
      for (int i = 0; i < 200; ++i) {
        if (signal_step()) break;
      }
      // signal stop
      signal_step(true);
      // join threads
      for (int i = 0; i < nworkers; ++i) workers[i].join();
    }

  private:
    void worker_function(int id, unsigned int nworkers) {
      bool stop = false;
      while (!stop) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this,id]{ return step > current_step[id] || step == -1;});
        stop = step == -1;
        lk.unlock();
        // do computation
        bool result = true;
        if (!stop) {
          result = computation_(current_step[id], id, nworkers);
        }
        // signal back that we are finished
        lk.lock();
        current_step[id] = step;
        stop_iteration[id] = result;
        lk.unlock();
        cv.notify_all();
      }
    }

    bool signal_step(bool stop = false) {
      // start new step
      {
        std::lock_guard<std::mutex> lk(m);
        if (stop) step = -1;
        else step += 1;
      }
      cv.notify_all();
      // wait until all workers haved finished their computation
      {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{ 
          return std::all_of(current_step.cbegin(), current_step.cend(), 
            [this](int i){ return i == step; });
        });
        return std::all_of(stop_iteration.cbegin(), stop_iteration.cend(), 
          [this](bool x){ return x;});
      }
    }

  private:
    T& computation_;
    std::condition_variable cv;
    std::mutex m;
    int step;
    std::vector<int> current_step;
    std::vector<bool> stop_iteration;
};

