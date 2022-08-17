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
#include <iostream>
#include <list>
#include <queue>
#include <vector>

// for time-consuming
class Timer {
 public:
  Timer() : time_start_(std::chrono::steady_clock::now()) {}
  void Reset() { time_start_ = std::chrono::steady_clock::now(); }
  // return int in milliseconds
  int Elapsed() const {
    auto time_now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_now -
                                                                 time_start_)
        .count();
  }

 private:
  std::chrono::time_point<std::chrono::steady_clock> time_start_;
};

// The Container based std::queue
template <typename T>
class BlockingQueue {
 public:
  BlockingQueue() {}
  ~BlockingQueue() {}

  void Push(std::vector<T> &&values) {
    for (auto &value : values) {
      queue_.push(std::move(value));
    }
  }

  std::vector<T> Pop(size_t num) {
    std::vector<T> block_data;
    while (block_data.size() < num) {
      block_data.push_back(std::move(queue_.front()));
      queue_.pop();
    }
    return block_data;
  }

 private:
  std::queue<T> queue_;
};

// The Container based std::vector
template <typename T>
class BlockingVector {
 public:
  BlockingVector() {}
  ~BlockingVector() {}

  void Push(std::vector<T> &&values) {
    vector_.insert(vector_.end(), make_move_iterator(values.begin()),
                   make_move_iterator(values.end()));
  }

  std::vector<T> Pop(size_t num) {
    std::vector<T> block_data;
    block_data.insert(block_data.end(), make_move_iterator(vector_.begin()),
                      make_move_iterator(vector_.begin() + num));
    // vector_.erase(vector_.begin(), vector_.begin() + num);
    return block_data;
  }

 private:
  std::vector<T> vector_;
};

// The Container based std::list
template <typename T>
class BlockingList {
 public:
  BlockingList() {}
  ~BlockingList() {}

  void Push(std::vector<T> &&values) {
    list_.insert(list_.end(), make_move_iterator(values.begin()),
                 make_move_iterator(values.end()));
  }

  std::vector<T> Pop(size_t num) {
    std::vector<T> block_data;
    block_data.insert(block_data.end(), make_move_iterator(list_.begin()),
                      make_move_iterator(list_.begin() + num));
    block_data.emplace_back();
    list_.erase(list_.begin(), list_.begin() + num);
    return block_data;
  }

 private:
  std::list<T> list_;
};

// source test data
const int kTotal = 100000;
const int kLength = 400;
void make_feats(std::vector<std::vector<float>> *feats_input) {
  for (int i = 0; i < kTotal; i++) {
    std::vector<float> feat;
    for (int j = 0; j < kLength; j++) {
      feat.push_back(j);
    }
    feats_input->push_back(std::move(feat));
  }
}

// We contrast the time consumed by BlockingQueue and BlockingVector.
template <typename T>
void test_process(T &&container) {
  int t;
  Timer timer;
  std::vector<std::vector<float>> feats_input;
  std::vector<std::vector<float>> feats_output;

  make_feats(&feats_input);

  timer.Reset();
  container.Push(std::move(feats_input));
  t = timer.Elapsed();
  std::cout << "\tPush duration: " << t << std::endl;

  timer.Reset();
  feats_output = std::move(container.Pop(kTotal / 3));
  t = timer.Elapsed();
  std::cout << "\tPop duration: " << t << std::endl;
}

int main() {
  BlockingQueue<std::vector<float>> queue;
  BlockingVector<std::vector<float>> vector;
  BlockingList<std::vector<float>> list;

  for (int i = 0; i < 3; i++) {
    std::cout << i << "-----------------" << std::endl;

    std::cout << "--using queue" << std::endl;
    test_process(std::move(queue));

    std::cout << "--using vector" << std::endl;
    test_process(std::move(vector));

    // std::cout << "--using list" << std::endl;
    // test_process(std::move(list));
  }

  return 0;
}
