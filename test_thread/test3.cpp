#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <condition_variable>

std::condition_variable cv;
std::mutex m;
int step = 0;
const int nworkers = 2;
std::array<int, nworkers> current_step = {0};


void worker_function(int id) {
  while (true) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [id]{ return step > current_step[id] || step == -1;});
    if (step == -1) {
      current_step[id] = step;
      cv.notify_all();
      break;
    }
    lk.unlock();
    // do computation
    std::cout << id << ": Starting computation\n";
    std::this_thread::sleep_for(std::chrono::seconds(id+1));
    std::cout << id << ": Finished computation\n";
    // signal back that we are finished
    lk.lock();
    current_step[id] += 1;
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
      bool res = true;
      for (int i = 0; i < nworkers; ++i) res &= current_step[i] >= step;
      return res;
    });
  }
}


int main(int argc, char* argv[]) {
  std::vector<std::thread> workers;
  for (int i = 0; i < nworkers; ++i) 
    workers.push_back(std::thread(worker_function, i));

  for (int i = 0; i < 2; ++i) {
    signal_step();
  }

  // signal stop
  signal_step(true);
  // join threads
  for (int i = 0; i < nworkers; ++i) workers[i].join();

  return 0; 
}
