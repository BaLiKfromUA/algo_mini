// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include "my_algorithm.h"

void my_copy_example() {
    const int N = 10;
    std::vector<int> from_vector(N);
    std::iota(from_vector.begin(), from_vector.end(), 1);

    for (int i: from_vector) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    std::vector<int> to_vector(N, 0);
    auto end = my_std::copy(from_vector.begin(), from_vector.end(),
                            to_vector.begin());
    assert(end == to_vector.end());
    for (int i: to_vector) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    int arr[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i: arr) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    int arr1[N];
    auto arr_end = my_std::copy(arr, arr + 10, arr1);
    assert(arr_end == arr1 + N);
    for (int i: arr1) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

void my_transform_example() {
    std::string s("hello");
    my_std::transform(s.begin(), s.end(), s.begin(),
                      [](unsigned char c) -> unsigned char { return std::toupper(c); });

    std::vector<std::size_t> ordinals;
    my_std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
                      [](unsigned char c) -> std::size_t { return c; });

    std::cout << s << ':';
    for (auto ord: ordinals) {
        std::cout << ' ' << ord;
    }

    my_std::transform(ordinals.cbegin(), ordinals.cend(), ordinals.cbegin(),
                      ordinals.begin(), std::plus<>{});

    std::cout << '\n';
    for (auto ord: ordinals) {
        std::cout << ord << ' ';
    }
    std::cout << '\n';

    std::vector<int> cubes{1, 2, 3, 4, 5, 6, 7, 8};
    my_std::transform(cubes.cbegin(), cubes.cend(), cubes.cbegin(), cubes.cbegin(),
                      cubes.begin(), [](int a, int b, int c) -> int { return a * b * c; });

    for (auto cube: cubes) {
        std::cout << cube << ' ';
    }
    std::cout << '\n';
}

void my_stream_example() {
    std::vector<int> init{1, 2, 3, 4, 5};
    // my_std::stream s({1, 2, 3, 4, 5});
    my_std::stream intStream(init);

    std::cout << intStream
            .map<int>([](const auto &value) { return value * value; })
            .reduce([](const auto &left, const auto &right) {
                return left + right;
            });


    std::cout << '\n';

    std::vector<std::string> strs{"balik", "mem", "heh", "longest", "test", "stream"};
    my_std::stream strStream(strs);

    std::cout << strStream.reduce([](const auto &left, const auto &right) {
        if (left.size() > right.size()) {
            return left;
        } else {
            return right;
        }
    }) << '\n';
}

int main() {
    //my_copy_example();
    //my_transform_example();
    my_stream_example();
    return 0;
}
