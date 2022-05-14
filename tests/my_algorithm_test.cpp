#include <gtest/gtest.h>
#include <numeric>
#include <set>
#include <fstream>
#include "my_algorithm.h"

const int N = 10;

TEST(Copy, BASIC_TEST_WITH_VECTORS_POD) {
    std::vector<int> from_vector(N);
    std::iota(from_vector.begin(), from_vector.end(), 1);

    // test initial values
    for (int i = 1; i <= N; ++i) {
        EXPECT_EQ(i, from_vector[i - 1]);
    }

    std::vector<int> to_vector(N, 0);

    // test initial values
    for (int i = 1; i <= N; ++i) {
        EXPECT_EQ(0, to_vector[i - 1]);
    }

    testing::internal::CaptureStdout();
    my_std::copy(from_vector.begin(), from_vector.end(),
                 to_vector.begin());
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("trivial", output);

    // test array after copy
    EXPECT_EQ(from_vector, to_vector);
    // change original vector
    from_vector[0] = 42;
    EXPECT_EQ(from_vector[0], 42);
    EXPECT_NE(from_vector, to_vector);
}

TEST(Copy, BASIC_TEST_WITH_ARRAYS_POD) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // test initial values
    for (int i = 1; i <= N; ++i) {
        EXPECT_EQ(i, arr[i - 1]);
    }

    int arr1[N]; // initial values -- garbage
    testing::internal::CaptureStdout();
    my_std::copy(arr, arr + 10, arr1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("trivial", output);

    // test array after copy
    EXPECT_NE(arr, arr1);
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], arr1[i]);
    }
}

TEST(Copy, NOT_TRIVIAL_OUTPUT_ITERATORS) {
    std::vector<int> from_vector(N);
    std::iota(from_vector.begin(), from_vector.end(), 1);

    testing::internal::CaptureStdout();
    my_std::copy(from_vector.begin(), from_vector.end(),
                 std::ostream_iterator<int>(std::cout, " "));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("not trivial1 2 3 4 5 6 7 8 9 10 ", output);

    testing::internal::CaptureStdout();
    std::vector<int> to_vector;
    my_std::copy(from_vector.begin(), from_vector.end(),
                 std::back_inserter(to_vector));
    output = testing::internal::GetCapturedStdout();

    EXPECT_EQ("not trivial", output);

    // test array after copy
    EXPECT_EQ(from_vector, to_vector);
    // change original vector
    from_vector[0] = 42;
    EXPECT_EQ(from_vector[0], 42);
    EXPECT_NE(from_vector, to_vector);
}

struct A {
    int a;

    explicit A(int i) {
        this->a = i;
    }

    A() {
        this->a = 0;
    };

    virtual void foo() {}
};

bool operator==(const A &lhs, const A &rhs) { return lhs.a == rhs.a; }

bool operator!=(const A &lhs, const A &rhs) { return !(lhs == rhs); }

TEST(Copy, BASIC_TEST_WITH_VECTORS_NOT_POD) {
    std::vector<A> from_vector = {A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9)};
    // test initial values
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(i, from_vector[i].a);
    }

    std::vector<A> to_vector(N);
    // test initial values
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(0, to_vector[i].a);
    }

    testing::internal::CaptureStdout();
    my_std::copy(from_vector.begin(), from_vector.end(),
                 to_vector.begin());
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("not trivial", output);

    // test array after copy
    EXPECT_EQ(from_vector, to_vector);
    // change original vector
    from_vector[0].a = 42;
    EXPECT_EQ(from_vector[0].a, 42);
    EXPECT_NE(from_vector, to_vector);
}

TEST(Transform, BASIC_TEST) {
    std::string s("hello");
    EXPECT_EQ("hello", s);
    my_std::transform(s.begin(), s.end(), s.begin(),
                      [](unsigned char c) -> unsigned char { return std::toupper(c); });
    EXPECT_EQ("HELLO", s);

    std::vector<std::size_t> ordinals;
    my_std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
                      [](unsigned char c) -> std::size_t { return c; });
    std::vector<std::size_t> expected = {72, 69, 76, 76, 79};
    EXPECT_EQ(expected, ordinals);

    my_std::transform(ordinals.cbegin(), ordinals.cend(), ordinals.cbegin(),
                      ordinals.begin(), std::plus<>{});
    expected = {144, 138, 152, 152, 158};
    EXPECT_EQ(expected, ordinals);

    std::vector<int> cubes{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> expected1 = {1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(expected1, cubes);

    my_std::transform(cubes.cbegin(), cubes.cend(), cubes.cbegin(), cubes.cbegin(),
                      cubes.begin(), [](int a, int b, int c) -> int { return a * b * c; });

    expected1 = {1, 8, 27, 64, 125, 216, 343, 512};
    EXPECT_EQ(expected1, cubes);
}

TEST(MapReduce, TEST_REDUCE) {
    std::vector<int> init = {1, 2, 3, 4, 5};
    int sum = my_std::stream(init).reduce(std::plus<>{});
    EXPECT_EQ(15, sum);

    std::vector<std::string> strs = {"h", "e", "l", "l", "o"};
    std::string concat = my_std::stream(strs).reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("hello", concat);

    std::set<std::string> strs_set(strs.begin(), strs.end());
    concat = my_std::stream(strs_set).reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("ehlo", concat); // sorted by letter

    using namespace std::string_literals;
    concat = my_std::stream({"h"s, "e"s, "l"s, "l"s, "o"s}).reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("hello", concat);
}

TEST(MapReduce, TEST_MAP) {
    // same type
    std::vector<int> init = {0, 1, 2, 3, 4};
    auto squares = my_std::stream(init).map<int>([](auto value) { return value * value; }).collect();
    std::vector<int> expected = {0, 1, 2, 3, 4};
    // test that we don't change initial data
    EXPECT_EQ(expected, init);
    expected = {0, 1, 4, 9, 16};
    EXPECT_EQ(expected, squares);

    // different type
    auto characters = my_std::stream(init).map<char>([](auto value) { return value + 'a'; }).collect();
    std::vector<char> expected_characters = {'a', 'b', 'c', 'd', 'e'};
    EXPECT_EQ(expected_characters, characters);

    // todo: test map with different structure (set, map)
}

TEST(MapReduce, TEST_EXAMPLES_FROM_TASK) {
    // sum of squares
    auto sum = my_std::stream({1, 2, 3, 4, 5})
            .map<int>([](const auto &value) { return value * value; })
            .reduce([](const auto &left, const auto &right) {
                return left + right;
            });
    EXPECT_EQ(55, sum);

    // longest string
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

    // words count
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
}

// todo: test collects