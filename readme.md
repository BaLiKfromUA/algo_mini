# Lab work 5: mini-library with algorithms

It is required to implement algorithms library C ++, which includes:

- Own implementation of std :: copy, which takes into account the difference between POD and non-POD.
- Implementation of transform for two and three intervals. (The interface should be similar to std :: transform).
- Implementation of **single-thread** MapReduce.

Author : [Valentyn Yukhymenko](https://github.com/BaLiKfromUA)

## Prerequisites

- **C++ compiler** - needs to support **C++17** standard
- **CMake** 3.15+

## Installing

1. Clone the project.
    ```bash
    git clone git@github.com:BaLiKfromUA/algo_mini.git
    ```
2. Install required packages.

   On Ubuntu:
   ```bash
   [[ -r dependencies/apt.txt ]] && sed 's/#.*//' dependencies/apt.txt | xargs sudo apt-get install -y
   ```
   On MacOS:
   ```bash
   [[ -r dependencies/homebrew.txt ]] && sed 's/#.*//' dependencies/homebrew.txt | xargs brew install
   ```
   Use Conan on Windows.
3. Build.
    ```bash
    cmake -Bbuild
    cmake --build build
    ```

### Results

API and implementation of library can be found in the header files.

#### Notes

Current Map-Reduce implementation supports next data structures **as an input**:
- selected stl containers: `std::vector, std::deque, std::list, std::set, std::multiset, std::array`;
- initializer list: `std::initializer_list<T>`
- raw array as the pair: `T *begin, std::size_t n`

Result of `collect()` can be packed to the next containers: `std::vector, std::deque, std::list, std::set, std::multiset`

By default it's `std::vector`

#### Examples

1. **my_std::copy**
```cpp
    std::vector<int> from_vector(N);
    std::iota(from_vector.begin(), from_vector.end(), 1);

    std::vector<int> to_vector(N, 0);
    auto end = my_std::copy(from_vector.begin(), from_vector.end(),
                            to_vector.begin());
    // test return value
    EXPECT_EQ(end, to_vector.end());
    // test array after copy
    EXPECT_EQ(from_vector, to_vector);

    // change original vector
    from_vector[0] = 42;
    EXPECT_EQ(from_vector[0], 42);
    EXPECT_NE(from_vector, to_vector);
```

2. **my_std::transform** for one and two interval(s)
```cpp
    std::string s("HELLO");

    std::vector<std::size_t> ordinals;
    my_std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
                        [](unsigned char c) -> std::size_t { return c; });
    std::vector<std::size_t> expected = {72, 69, 76, 76, 79};
    EXPECT_EQ(expected, ordinals);

    my_std::transform(ordinals.cbegin(), ordinals.cend(), ordinals.cbegin(),
    ordinals.begin(), std::plus<>{});
    expected = {144, 138, 152, 152, 158};
    EXPECT_EQ(expected, ordinals);
```
3. **my_std::transform** for three intervals (hope I understand task correctly):
```cpp
    std::vector<int> cubes{1, 2, 3, 4, 5, 6, 7, 8};

    my_std::transform(cubes.cbegin(), cubes.cend(), cubes.cbegin(), cubes.cbegin(),
                      cubes.begin(), [](int a, int b, int c) -> int { return a * b * c; });

    std::vector<int> expected = {1, 8, 27, 64, 125, 216, 343, 512};
    EXPECT_EQ(expected, cubes);
```
4. **my_std::stream** -- MapReduce inspired by Java Stream API :)
   1. evaluate sum of squares from array of integers
   ```cpp
   auto sum = my_std::stream({1, 2, 3, 4, 5})
                        .map<int>([](const auto &value) { return value * value; })
                        .reduce(std::plus<>{});
   EXPECT_EQ(55, sum);
   ```
   2. find the longest string in array
   ```cpp
   using namespace std::string_literals;
   auto longest = my_std::stream({"balik"s, "mem"s, "heh"s, "longest"s, "test"s, "stream"s})
                        .reduce([](const auto &left, const auto &right) {
                            if (left.size() > right.size()) {
                                return left;
                            } else {
                                return right;
                            }
                        });
   EXPECT_EQ("longest", longest);
   ```
   3. calculate word frequency for all words inside the given text files
   ```cpp
    my_std::stream fileStream({"../data/file1.txt", "../data/file2.txt", "../data/file3.txt"});

    auto wordCnt = fileStream
            .map<std::map<std::string, int>>([](const auto &value) {
                std::map<std::string, int> cnt;
                std::ifstream in(value);
                if (in) {
                    std::string s;
                    while (in >> s) {
                        ++cnt[s];
                    }
                }
                return cnt;
            })
            .reduce([](const auto &left, const auto &right) {
                std::map<std::string, int> result = left;

                for (const auto &[key, value]: right) {
                    result[key] += value;
                }

                return result;
            });

    std::vector<std::pair<std::string, int>> expected = {{"one",   1},
                                                         {"two",   2},
                                                         {"three", 3},
                                                         {"four",  4},
                                                         {"five",  5},
                                                         {"six",   6},
                                                         {"seven", 7}};

    EXPECT_EQ(expected.size(), wordCnt.size());
    for (const auto &[key, value]: expected) {
        EXPECT_EQ(value, wordCnt[key]);
    }
   ```
