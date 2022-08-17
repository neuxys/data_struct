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

namespace terroo {
struct Coordinates {
  int x, y;
};
}  // namespace terroo

int main() {
  std::vector<terroo::Coordinates> v1 = {{0, 0}, {0, 1}, {0, 2}}, v2, v3;

  for (size_t i = 0; i < v1.size(); ++i) {
    v2.push_back({1, v1[i].y});
    v3.push_back({2, v1[i].y});
  }

  for (size_t i = 0; i < v1.size(); ++i) {
    std::cout << v1[i].x << ',' << v1[i].y << ' ';
  }
  std::cout << '\n';

  for (size_t i = 0; i < v2.size(); ++i) {
    std::cout << v2[i].x << ',' << v2[i].y << ' ';
  }
  std::cout << '\n';

  for (size_t i = 0; i < v3.size(); ++i) {
    std::cout << v3[i].x << ',' << v3[i].y << ' ';
  }
  std::cout << '\n';

  std::cout << "------" << '\n';

  std::move(v1.begin(), v1.begin() + 1, v2.begin() + 1);
  std::move(v1.begin(), v1.begin() + 1, v3.begin() + 2);

  for (size_t i = 0; i < v1.size(); ++i) {
    std::cout << v1[i].x << ',' << v1[i].y << ' ';
  }
  std::cout << '\n';

  for (size_t i = 0; i < v2.size(); ++i) {
    std::cout << v2[i].x << ',' << v2[i].y << ' ';
  }
  std::cout << '\n';

  for (size_t i = 0; i < v3.size(); ++i) {
    std::cout << v3[i].x << ',' << v3[i].y << ' ';
  }
  std::cout << '\n';

  return 0;
}
