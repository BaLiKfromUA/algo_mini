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