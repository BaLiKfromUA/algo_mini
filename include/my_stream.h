// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_MY_STREAM_H
#define TEMPLATE_MY_STREAM_H

#include <vector>
#include <initializer_list>
#include <functional>

namespace my_std {
    template<typename T, template<typename...> typename Container = std::vector>
    class stream {
    public:
        explicit stream(const Container<T> &container) : _container(container) {}

        stream(std::initializer_list<T> list) : _container(Container<T>(list)) {}

        stream(T *begin, std::size_t n) : _container(Container<T>(begin, begin + n)) {}

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

        template<template<typename...> typename OutputContainer = std::vector>
        OutputContainer<T> collect() {
            OutputContainer<T> result;

            for (const auto &elm: _container) {
                result.insert(result.end(), elm);
            }

            return result;
        }

    private:
        const Container<T> _container;
    };
}

#endif //TEMPLATE_MY_STREAM_H
