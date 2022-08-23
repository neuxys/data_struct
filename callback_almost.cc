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

class Base {
 public:
  Base() {}
  virtual ~Base() {}

  void get_val() { callback(val_); }

 protected:
  virtual void callback(int val) = 0;

 private:
  int val_ = 8;
};

class Child : public Base {
 public:
  Child() {}
  virtual ~Child() {}

 protected:
  virtual void callback(int val) {
    std::cout << "We get value = " << val << " in Child" << std::endl;
  }
};

/**
 * Using inheritance to implement callback function
 * */
int main() {
  Child child;
  child.get_val();
  return 0;
}
