// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_MY_STREAM_H
#define TEMPLATE_MY_STREAM_H

#include <vector>
#include <initializer_list>
#include <functional>
#include <set>
#include <forward_list>
#include <list>
#include <deque>

namespace {
    template<typename T>
    struct is_supported_stl_container_impl : std::false_type {
    };
    template<typename... Args>
    struct is_supported_stl_container_impl<std::vector<Args...>> : std::true_type {
    };
    template<typename... Args>
    struct is_supported_stl_container_impl<std::deque<Args...>> : std::true_type {
    };
    template<typename... Args>
    struct is_supported_stl_container_impl<std::list<Args...>> : std::true_type {
    };
    template<typename... Args>
    struct is_supported_stl_container_impl<std::set<Args...>> : std::true_type {
    };
    template<typename... Args>
    struct is_supported_stl_container_impl<std::multiset<Args...>> : std::true_type {
    };

    template<typename T>
    struct is_supported_stl_container {
        static constexpr bool const value = is_supported_stl_container_impl<std::decay_t<T>>::value;
    };
}

namespace my_std {
    template<typename T, template<typename...> typename Container = std::vector,
            class = typename std::enable_if<is_supported_stl_container<Container<T>>::value>>
    class stream {
    private:
        const Container<T> _container;
    public:
        explicit stream(const Container<T> &container) : _container(container) {}

        stream(std::initializer_list<T> list) : _container(Container<T>(list)) {}

        stream(T *begin, std::size_t n) : _container(Container<T>(begin, begin + n)) {}

        template<size_t N>
        explicit stream(std::array<T, N> arr) : _container(Container<T>(arr.begin(), arr.end())) {}

        template<typename U>
        stream<U> map(std::function<U(const T &)> func) {
            Container<U> mappedValues;

            for (const auto &elm: _container) {
                mappedValues.insert(mappedValues.end(), func(elm));
            }

            return stream<U, Container>(mappedValues);
        }

        T reduce(std::function<T(const T &, const T &)> reducer, const T &startValue = T{}) {
            T result = startValue;

            for (const auto &elm: _container) {
                result = reducer(result, elm);
            }

            return result;
        }

        template<template<typename...> typename OutputContainer = Container,
                class = typename std::enable_if<is_supported_stl_container<OutputContainer<T>>::value>>
        OutputContainer<T> collect() {
            OutputContainer<T> result;

            for (const auto &elm: _container) {
                result.insert(result.end(), elm);
            }

            return result;
        }
    };
}

#endif //TEMPLATE_MY_STREAM_H
