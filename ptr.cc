/*
 Copyright 2022 neuxys@gmail.com

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <iostream>
#include <memory>
#include <string>
#include <variant>

/**
 * unique_ptr
 * https://changkun.de/modern-cpp/zh-cn/05-pointers/#5-3-std-unique-ptr
 * */

class Foo {
 public:
  Foo() { std::cout << "construct" << std::endl; }
  ~Foo() { std::cout << "deconstruct" << std::endl; }

  void foo() { std::cout << "foo function" << std::endl; }
};

void f(const Foo &) { std::cout << "f(const Foo&)" << std::endl; }

void unique_ptr_example() {
  std::unique_ptr<Foo> p1 = std::make_unique<Foo>();

  if (p1) p1->foo();

  {
    std::unique_ptr<Foo> p2 = std::move(p1);
    f(*p2);

    if (p2) p2->foo();

    if (p1) p1->foo();

    p1 = std::move(p2);

    if (p2) p2->foo();
  }

  if (p1) p1->foo();
}

/**
 * shared_ptr cause memory leak
 * https://changkun.de/modern-cpp/zh-cn/05-pointers/#5-4-std-weak-ptr
 * */
struct A;
struct B;

struct A {
  std::shared_ptr<B> pointer;
  ~A() { std::cout << "A 被销毁" << std::endl; }
};

struct B {
  std::shared_ptr<A> pointer;
  ~B() { std::cout << "B 被销毁" << std::endl; }
};

void weak_ptr_example() {
  auto a = std::make_shared<A>();
  auto b = std::make_shared<B>();

  // memory will leak due to inter-reference between A and B
  a = b->pointer;
  b = a->pointer;
}

/**
 * weak_ptr to prevent memory leak
 * https://en.cppreference.com/w/cpp/memory/weak_ptr/~weak_ptr#Example
 * */
class Node {
  char id;
  std::variant<std::weak_ptr<Node>, std::shared_ptr<Node>> ptr;

 public:
  Node(char id) : id{id} {}
  ~Node() { std::cout << "  '" << id << "' reclaimed\n"; }
  /*...*/
  void assign(std::weak_ptr<Node> p) { ptr = p; }
  void assign(std::shared_ptr<Node> p) { ptr = p; }
};

enum class shared { all, some };

void test_cyclic_graph(const shared x) {
  auto A = std::make_shared<Node>('A');
  auto B = std::make_shared<Node>('B');
  auto C = std::make_shared<Node>('C');

  A->assign(B);
  B->assign(C);

  if (shared::all == x) {
    C->assign(A);
    std::cout << "All links are shared pointers";
  } else {
    C->assign(std::weak_ptr<Node>(A));
    std::cout << "One link is a weak_ptr";
  }
  /*...*/
  std::cout << "\nLeaving...\n";
}

void test_weak_ptr() {
  test_cyclic_graph(shared::some);
  test_cyclic_graph(shared::all);  // produces a memory leak
}

/**
 * main test
 * */
int main() {
  // unique_ptr_example();

  // weak_ptr_example();

  test_weak_ptr();
}
