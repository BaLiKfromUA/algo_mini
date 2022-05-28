#include <gtest/gtest.h>
#include <numeric>
#include <set>
#include <fstream>
#include "my_algorithm.h"

TEST(MapReduce, TEST_REDUCE) {
    std::vector<int> init = {1, 2, 3, 4, 5};
    int sum = my_std::stream(init)
            .reduce(std::plus<>{});
    EXPECT_EQ(15, sum);

    std::vector<std::string> strs = {"h", "e", "l", "l", "o"};
    std::string concat = my_std::stream(strs)
            .reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("hello", concat);

    std::set<std::string> strs_set(strs.begin(), strs.end());
    concat = my_std::stream(strs_set)
            .reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("ehlo", concat); // sorted by letter

    using namespace std::string_literals;
    concat = my_std::stream({"h"s, "e"s, "l"s, "l"s, "o"s})
            .reduce([](auto left, auto right) { return left + right; });
    EXPECT_EQ("hello", concat);

    using namespace std::string_literals;
    std::string arr[] = {"h"s, "e"s, "l"s, "l"s, "o"s};
    concat = my_std::stream(arr, 5)
            .reduce([](auto left, auto right) { return left + right; });
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
}

TEST(MapReduce, TEST_COLLECT) {
    std::vector<int> vector = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    auto stream = my_std::stream(vector);

    auto collectedVector = stream.collect();
    EXPECT_EQ(vector.size(), collectedVector.size());

    for (size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(vector[i], collectedVector[i]);
    }

    auto collectedSet = stream.collect<std::set>();
    EXPECT_EQ(5, collectedSet.size());

    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(collectedSet.count(i) > 0);
    }

    auto collectedMultiSet = stream.collect<std::multiset>();
    EXPECT_EQ(10, collectedMultiSet.size());
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(collectedMultiSet.count(i) > 0);
    }

    auto collectedDeque = stream.collect<std::deque>();
    EXPECT_EQ(10, collectedDeque.size());
    for (int &i: vector) {
        auto front = collectedDeque.front();
        collectedDeque.pop_front();
        EXPECT_EQ(i, front);
    }
    EXPECT_EQ(0, collectedDeque.size());

    auto collectedList = stream.collect<std::list>();
    EXPECT_EQ(10, collectedList.size());
    for (int &i: vector) {
        auto front = collectedList.front();
        collectedList.pop_front();
        EXPECT_EQ(i, front);
    }
    EXPECT_EQ(0, collectedList.size());
}

TEST(MapReduce, TEST_EXAMPLES_FROM_TASK) {
    // sum of squares
    auto sum = my_std::stream({1, 2, 3, 4, 5})
            .map<int>([](const auto &value) { return value * value; })
            .reduce(std::plus<>{});
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