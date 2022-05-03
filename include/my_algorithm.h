// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H
#define LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H

#include <cassert>
#include <type_traits>
#include <iterator>
#include <memory>
#include "my_stream.h" // just in order to to use one header in main.cpp

namespace my_std {
    // todo: add separate iterator type for output
    // https://en.cppreference.com/w/cpp/algorithm/copy
    template<typename iterator>
    iterator copy(iterator first, iterator last, iterator d_first) {
        using T = typename std::iterator_traits<iterator>::value_type;

        if constexpr(std::is_trivially_copyable_v<T>) {
            const auto len = last - first;
            memcpy(&*d_first, &*first, len * sizeof(T));
            return d_first + len;
        } else {
            while (first != last) {
                *d_first = *first;
                ++first;
                ++d_first;
            }

            return d_first;
        }
    }

    template<typename InputIt, typename OutputIt, typename UnaryOperation>
    OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
        while (first1 != last1) {
            *d_first++ = unary_op(*first1++);
        }
        return d_first;
    }

    template<typename InputIt1, typename InputIt2, typename OutputIt, typename BinaryOperation>
    OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first,
                       BinaryOperation binary_op) {
        while (first1 != last1) {
            *d_first++ = binary_op(*first1++, *first2++);
        }
        return d_first;
    }

    template<typename InputIt1, typename InputIt2, typename InputIt3, typename OutputIt, typename TernaryOperation>
    OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt3 first3,
                       OutputIt d_first, TernaryOperation ternary_op) {
        while (first1 != last1) {
            *d_first++ = ternary_op(*first1++, *first2++, *first3++);
        }
        return d_first;
    }
}

#endif //LAB9_APPS_ALGORITHMS_BALIKFROMUA_MY_ALGORITHM_H
