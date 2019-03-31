#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <algorithm>
#include <condition_variable>

std::condition_variable cv;
std::mutex m;
int step = 0;
std::vector<int> current_step;


void worker_function(int id) {
  bool stop = false;
  while (!stop) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [id]{ return step > current_step[id] || step == -1;});
    stop = step == -1;
    lk.unlock();
    // do computation
    if (!stop) {
      std::cout << id << ": Starting computation\n";
      std::this_thread::sleep_for(std::chrono::seconds(id+1));
      std::cout << id << ": Finished computation\n";
    }
    // signal back that we are finished
    lk.lock();
    current_step[id] = step;
    lk.unlock();
    cv.notify_all();
  }
}

void signal_step(bool stop = false) {
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
    cv.wait(lk, []{ 
      return std::all_of(current_step.cbegin(), current_step.cend(), 
        [](int i){ return i == step; });
    });
  }
}


int main(int argc, char* argv[]) {
  int nworkers = 4;

  std::vector<std::thread> workers;
  for (int i = 0; i < nworkers; ++i) {
    current_step.push_back(0);
    workers.push_back(std::thread(worker_function, i));
  }

  for (int i = 0; i < 2; ++i) {
    signal_step();
  }

  // signal stop
  signal_step(true);
  // join threads
  for (int i = 0; i < nworkers; ++i) workers[i].join();

  return 0; 
}