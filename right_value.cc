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
#include <string>
#include <utility>
#include <vector>

class A {
 public:
  A() : pointer(new int(1)) {
    std::cout << "construct" << pointer << std::endl;
  }

  A(A& a) : pointer(new int(*a.pointer)) {
    std::cout << "copy" << pointer << std::endl;
  }

  A(A&& a) : pointer(a.pointer) {
    a.pointer = nullptr;
    std::cout << "move" << pointer << std::endl;
  }

  ~A() {
    std::cout << "deconstruct" << std::endl;
    delete pointer;
  }

 public:
  int* pointer;
};

A return_lvalue(bool test) {
  A a, b;
  if (test)
    // 等价于 static_cast<A&&>(a);
    return a;
  else
    // 等价于 static_cast<A&&>(b);
    return b;
}

void test_move_implement() {
  A obj = return_lvalue(false);

  std::cout << "obj:" << std::endl;

  std::cout << obj.pointer << std::endl;

  std::cout << *obj.pointer << std::endl;
}

void test_move_std() {
  std::string str = "Hello World";
  std::vector<std::string> v;

  // 将使用 push_back(const T&), 即产生拷贝行为
  v.push_back(str);
  // 将输出 "str: Hello world."
  std::cout << "str:" << str << std::endl;

  // 将使用 push_back(const T&&), 不会出现拷贝行为
  // 而整个字符串会被移动到 vector 中，
  // 所以有时候 std::move 会用来减少拷贝出现的开销
  // 这步操作后, str 中的值会变为空
  v.push_back(move(str));
  // 将输出 "str: "
  std::cout << "str:" << str << std::endl;

  return;
}

int main() {
  // test_move_implement();

  test_move_std();

  return 0;
}
