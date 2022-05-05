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
    private:
        const Container<T> _container{};

    public:
        explicit stream(const Container<T> &container) : _container(container) {}

        stream(std::initializer_list<T> list) : _container(Container<T>(list)) {}

        // todo: constructor for raw array and for two pointers

        template<typename U>
        stream<U> map(std::function<U(const T &)> func) {
            std::vector<U> mappedValues;

            for (const auto &elm: _container) {
                mappedValues.push_back(func(elm));
            }

            return stream<U, std::vector>(mappedValues);
        }

        // todo: fix to more flexible
        Container<T> collect() {
            return _container;
        }

        T reduce(std::function<T(const T &, const T &)> reducer, const T &startValue = T{}) {
            T result = startValue;

            for (const auto &elm: _container) {
                result = reducer(result, elm);
            }

            return result;
        }

    };
}

#endif //TEMPLATE_MY_STREAM_H
