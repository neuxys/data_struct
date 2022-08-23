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

#include <iostream>
#include <mutex>   // NOLINT [build/c++11]
#include <thread>  // NOLINT [build/c++11]

void using_pthread_create() {
  std::thread t([]() { std::cout << "subthread.." << std::endl; });

  t.join();
}

// unique_lock example
int v = 0;

void critical_section(int change_v) {
  static std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  // 执行竞争操作
  v = change_v;
  std::cout << "v: " << v << std::endl;

  // 离开此作用域mtx会自动释放
}

void using_unique_lock() {
  std::thread t1(critical_section, 2), t2(critical_section, 3);

  t1.join();
  t2.join();

  std::cout << v << std::endl;
}

int v2 = 0;
void critical_section2(int change_v) {}

int main() {
  // using_pthread_create();

  using_unique_lock();
  return 0;
}
