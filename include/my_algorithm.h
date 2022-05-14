// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H
#define LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H

#ifdef TEST_DEBUG

#include <iostream>

#endif

#include <cassert>
#include <type_traits>
#include <iterator>
#include <memory>
#include <cstring>
#include "my_stream.h" // just in order to use one header in main.cpp

namespace my_std {

    namespace {
        template<typename Base, typename Check, typename... R>
        struct are_all_base_of {
            constexpr static bool value =
                    std::is_base_of<Base, typename std::iterator_traits<Check>::iterator_category>::value &&
                    are_all_base_of<Base, R...>::value;
        };

        template<typename Base, typename Check>
        struct are_all_base_of<Base, Check> {
            constexpr static bool value = std::is_base_of<Base, typename std::iterator_traits<Check>::iterator_category>::value;
        };
    }

    // https://en.cppreference.com/w/cpp/algorithm/copy
    template<typename InputIt, typename OutputIt,
            class = typename std::enable_if<
                    are_all_base_of<std::input_iterator_tag, InputIt>::value
                    &&
                    are_all_base_of<std::output_iterator_tag, OutputIt>::value>>
    void copy(InputIt first, InputIt last, OutputIt d_first) {
        using T = typename std::iterator_traits<InputIt>::value_type;
        using U = typename std::iterator_traits<OutputIt>::value_type;

        if constexpr(std::is_trivially_copyable_v<T> && std::is_same_v<T, U>) {
#ifdef TEST_DEBUG
            std::cout << "trivial";
#endif
            auto len = last - first;
            memcpy(&*d_first, &*first, len * sizeof(T));
        } else {
#ifdef TEST_DEBUG
            std::cout << "not trivial";
#endif

            while (first != last) {
                *d_first = *first;
                ++first;
                ++d_first;
            }
        }
    }

    // https://en.cppreference.com/w/cpp/algorithm/transform
    template<typename InputIt, typename OutputIt, typename UnaryOperation,
            class = typename std::enable_if<
                    are_all_base_of<std::input_iterator_tag, InputIt>::value
                    &&
                    are_all_base_of<std::output_iterator_tag, OutputIt>::value>>
    OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
        while (first1 != last1) {
            *d_first++ = unary_op(*first1++);
        }
        return d_first;
    }

    template<typename InputIt1, typename InputIt2, typename OutputIt, typename BinaryOperation,
            class = typename std::enable_if<
                    are_all_base_of<std::input_iterator_tag, InputIt1, InputIt2>::value
                    &&
                    are_all_base_of<std::output_iterator_tag, OutputIt>::value>>
    OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first,
                       BinaryOperation binary_op) {
        while (first1 != last1) {
            *d_first++ = binary_op(*first1++, *first2++);
        }
        return d_first;
    }

    template<typename InputIt1, typename InputIt2, typename InputIt3, typename OutputIt, typename TernaryOperation,
            class = typename std::enable_if<
                    are_all_base_of<std::input_iterator_tag, InputIt1, InputIt2, InputIt3>::value
                    &&
                    are_all_base_of<std::output_iterator_tag, OutputIt>::value>>
    OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt3 first3,
                       OutputIt d_first, TernaryOperation ternary_op) {
        while (first1 != last1) {
            *d_first++ = ternary_op(*first1++, *first2++, *first3++);
        }
        return d_first;
    }

}

#endif //LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H
