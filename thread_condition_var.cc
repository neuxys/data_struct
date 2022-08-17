// Copyright 2022 neuxys@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main() {
  std::queue<int> produced_num;
  std::mutex mtx;
  std::condition_variable cv;
  bool notified = false;

  // 生产者
  auto producer = [&]() {
    for (int i = 0;; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(900));
      std::unique_lock<std::mutex> lock(mtx);
      std::cout << "producing: " << i << std::endl;
      produced_num.push(i);
      notified = true;
      cv.notify_all();
    }
  };

  // 消费者
  auto customer = [&]() {
    while (true) {
      std::unique_lock<std::mutex> lock(mtx);
      while (!notified) {
        cv.wait(lock);  // 防止虚假唤醒
      }

      // 短暂取消锁，使得生产者能在消费者消费空前继续生产
      lock.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      lock.lock();

      while (!produced_num.empty()) {
        std::cout << "consuming ... " << produced_num.front() << std::endl;
        produced_num.pop();
      }
      notified = false;
    }
  };

  // 分别在不同的线程中运行
  std::thread p(producer);
  std::thread cs[2];
  for (int i = 0; i < 2; i++) {
    cs[i] = std::thread(customer);
  }

  p.join();
  for (int j = 0; j < 2; j++) {
    cs[j].join();
  }

  return 0;
}
