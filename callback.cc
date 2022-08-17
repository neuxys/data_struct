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

/**
 * !!!!!!!!!!!!!!!!!!!!!!
 * !!!!!  important !!!!!
 * !!!!!!!!!!!!!!!!!!!!!!
 * This test is the answer of "How to safely use callbacks when the bound
 * function could be deleted"
 *
 * There are two method to solve the question.
 * https://stackoverflow.com/questions/61360300/how-to-safely-use-callbacks-when-the-bound-function-could-be-deleted
 *
 */
#include <functional>
#include <iostream>
#include <memory>

// Method 1: Test for weak_ptr.lock -----------------------------
/**
 * std::weak_ptr<T>::lock:
 * Creates a new std::shared_ptr that shares ownership of the managed object. If
 * there is no managed object, i.e. *this is empty, then the returned shared_ptr
 * also is empty.
 *
 * Effectively returns:
 * expired() ? shared_ptr<T>() : shared_ptr<T>(*this), executed atomically.
 */

#include <functional>
#include <iostream>
#include <memory>

void observe(std::weak_ptr<int> weak) {
  if (auto observe = weak.lock()) {
    std::cout << "\tobserve() able to lock weak_ptr<>, value=" << *observe
              << "\n";
  } else {
    std::cout << "\tobserve() unable to lock weak_ptr<>\n";
  }
}

int TestWeakPtr() {
  std::weak_ptr<int> weak;
  std::cout << "weak_ptr<> not yet initialized\n";
  observe(weak);

  {
    auto shared = std::make_shared<int>(42);
    weak = shared;
    std::cout << "weak_ptr<> initialized with shared_ptr.\n";
    observe(weak);
  }

  std::cout << "shared_ptr<> has been destructed due to scope exit.\n";
  observe(weak);

  return 0;
}

// Method 2: Test for shared_ptr -----------------------------
/**
 * You are extending the lifetime of an object not preventing the function to be
 * called when the pointer has been reset.
 * */

class ABCD : public std::enable_shared_from_this<ABCD> {
 public:
  void call_me_anytime() { std::cout << "Thanks for Calling Me" << std::endl; }

 public:
  ABCD(void) { std::cout << "CONSTRUCTOR" << std::endl; }

  ~ABCD(void) { std::cout << "DESTRUCTOR" << std::endl; }
};

void TestSharedPtr() {
  auto ptr = std::make_shared<ABCD>();
  auto cb = std::bind(&ABCD::call_me_anytime, ptr->shared_from_this());
  ptr.reset();

  std::cout << "RESETING SHARED_PTR" << std::endl;
  std::cout << "CALLING CALLBACK" << std::endl;
  cb();
  std::cout << "RETURNING" << std::endl;
}

// Method 3: Lambda with a reference to the unique_ptr or shared_ptr

class A {
 public:
  A() {}
  ~A() {}
  int var = 10;
  void check() { std::cout << "var is " << var << std::endl; }
};

void LambdaWithReferencePtr() {
  auto a_unique_ptr = std::make_unique<A>();
  auto a_shared_ptr = std::make_shared<A>();
  std::function<void()> f = [&] {
    if (a_unique_ptr) {
      std::cout << "we can call with a_unique_ptr" << std::endl;
      a_unique_ptr->check();
    } else {
      std::cout << "we can not call with a_unique_ptr" << std::endl;
    }

    if (a_shared_ptr) {
      std::cout << "we can call with a_shared_ptr" << std::endl;
      a_shared_ptr->check();
    } else {
      std::cout << "we can not call with a_shared_ptr" << std::endl;
    }
  };
  a_unique_ptr.reset();
  a_shared_ptr.reset();
  f();
}

int main(void) {
  TestWeakPtr();             // good method
  TestSharedPtr();           // bad method due to expend lifetime
  LambdaWithReferencePtr();  // good method
  return 0;
}
