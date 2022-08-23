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

#pragma pack(4)
#include <functional>
#include <iostream>
#include <mutex>   // NOLINT [build/c++11]
#include <thread>  // NOLINT [build/c++11]
#include <vector>

struct A {
  char a;
  int b;
  short c;  // NOLINT [build/c++11]
};

struct X1 {
  int i;
  char c1;
  char c2;
};

struct X2 {
  char c1;
  int i;
  char c2;
};

struct X3 {
  char c1;
  char c2;
  int i;
};

struct X4 {
  char c1;
  short s1;  // NOLINT [build/c++11]
  short s2;  // NOLINT [build/c++11]
  int i1;
};

struct X5 {
  int i1;
  short s1;  // NOLINT [build/c++11]
  short s2;  // NOLINT [build/c++11]
  char c1;
};

struct X6 {
  char a;         // 1 byte
  char _pad0[3];  // padding to put 'b' on 4-byte boundary
  int b;          // 4 bytes
  short c;        // 2 bytes  // NOLINT [build/c++11]
  char d;         // 1 byte
  char _pad1[1];  // padding to make sizeof(x_) multiple of 4
};

enum EnumType {
  kEndBatch = 0x00,  // End of current decoding batch, normal case
  kEndpoint = 0x01,  // Endpoint is detected
  kEndFeats = 0x02,  // All feature is decoded
  kWaitFeats = 0x03  // Feat is not enough for one chunk inference, wait
};

int main() {
  using ResultCallback_t = std::function<void(const char* result, size_t len)>;
  struct A a;
  struct X1 x1;
  struct X2 x2;
  struct X3 x3;
  struct X4 x4;
  struct X5 x5;
  struct X6 x6;
  EnumType enumType_;
  std::vector<std::string> vector_str_;
  std::string str;
  ResultCallback_t func_;
  std::mutex mutex_;
  std::thread thread_;
  std::shared_ptr<int> shared_ptr_int_;
  std::shared_ptr<int64_t> shared_ptr_int64_;
  std::weak_ptr<int> weak_ptr_;
  std::cout << "A: " << sizeof(a) << ", X1: " << sizeof(x1)
            << ", X2: " << sizeof(x2) << ", X3: " << sizeof(x3)
            << ", X4: " << sizeof(x4) << ", X5: " << sizeof(x5)
            << ", X6: " << sizeof(x6) << ", enumType_: " << sizeof(enumType_)
            << ", vector_str_: " << sizeof(vector_str_)
            << ", str: " << sizeof(str) << ", func_: " << sizeof(func_)
            << ", mutex_: " << sizeof(mutex_)
            << ", thread_: " << sizeof(thread_)
            << ", shared_ptr_int_: " << sizeof(shared_ptr_int_)
            << ", shared_ptr_int64_: " << sizeof(shared_ptr_int64_)
            << ", weak_ptr_: " << sizeof(weak_ptr_) << std::endl;
  return 0;
}