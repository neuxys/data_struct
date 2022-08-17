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
#include <vector>

#include "blocking_queue.h"
#include "timer.h"

void UseElement(wenet::BlockingQueue<std::vector<float>> *q,
                const std::vector<std::vector<float>> &feats,
                std::vector<std::vector<float>> *out) {
  wenet::Timer timer;
  // 写入
  for (size_t i = 0; i < feats.size(); ++i) {
    q->Push(std::move(feats[i]));
  }
  int t = timer.Elapsed();
  std::cout << "element_w_t: " << t << std::endl;

  // 读取
  timer.Reset();
  while (!q->Empty()) {
    out->push_back(std::move(q->Pop()));
  }
  t = timer.Elapsed();
  std::cout << "element_r_t: " << t << std::endl;
}

void UseArray(wenet::BlockingQueue<std::vector<float>> *q,
              const std::vector<std::vector<float>> &feats,
              std::vector<std::vector<float>> *out) {
  wenet::Timer timer;
  // 写入
  q->Append(feats);
  int t = timer.Elapsed();
  std::cout << "array_w_t: " << t << std::endl;

  // 读取
  timer.Reset();
  q->Drain(out);
  t = timer.Elapsed();
  std::cout << "array_r_t: " << t << std::endl;
}

int main() {
  std::vector<std::vector<float>> feats;

  int frame_num = 200;
  int frame_len = 80000;
  for (int i = 0; i < frame_len; ++i) {
    std::vector<float> feat;
    for (int j = 0; j < frame_num; ++j) {
      feat.push_back(j);
    }
    feats.push_back(std::move(feat));
  }

  std::cout << "start " << std::endl;

  wenet::BlockingQueue<std::vector<float>> q1;
  std::vector<std::vector<float>> out1;
  std::cout << "before: array out1.size(): " << out1.size() << std::endl;
  UseElement(&q1, feats, &out1);
  std::cout << "element out1.size(): " << out1.size()
            << ", array out1[0].size(): " << out1[0].size() << std::endl;

  wenet::BlockingQueue<std::vector<float>> q2;
  std::vector<std::vector<float>> out2;
  std::cout << "before: array out2.size(): " << out2.size() << std::endl;
  UseArray(&q2, feats, &out2);
  std::cout << "after: array out2.size(): " << out2.size() << std::endl
            << "array out2[0].size(): " << out2[0].size() << std::endl;

  return 0;
}
