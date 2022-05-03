//
// Created by balik on 5/3/2022.
//

#ifndef TEMPLATE_MY_STREAM_H
#define TEMPLATE_MY_STREAM_H

#include <vector>
#include <initializer_list>
#include <functional>

namespace my_std {
    template<typename T, template<typename...> typename Container = std::vector>
    class stream {
    private:
        Container<T> _container{};

    public:
        explicit stream(const Container<T> &container) : _container(container) {}

        stream(std::initializer_list<T> list) {
            _container = Container<T>(list);
        }

        // todo: constructor for raw array and for two pointers

        stream<T, Container> &map(std::function<T(const T &)> func) {
            auto index = _mapOperations.size();
            _mapOperations.push_back(func);
            _pipeline.push_back(Operation::makeOperation(index, Type::Map));
            return *this;
        }

        Container<T> collect() {
            Container<T> result;

            for (const auto &value: _container) {
                T tmp = value;

                for (const auto &op: _pipeline) {
                    switch (op.type) {
                        case Type::Map:
                            tmp = _mapOperations[op.index](tmp);
                            break;
                    }
                }

                result.insert(result.end(), tmp);
            }

            return result;
        }

        T reduce(std::function<T(const T &, const T &)> reducer, const T &startValue = T{}) {
            T result = startValue;
            auto mappedValues = collect();
            for (const auto &elm: mappedValues) {
                result = reducer(result, elm);
            }
            return result;
        }

    private:
        enum class Type {
            Map
        };

        struct Operation {
            size_t index;
            Type type;

            static Operation makeOperation(size_t index, Type type) {
                Operation operation;
                operation.index = index;
                operation.type = type;
                return operation;
            }
        };

        std::vector<std::function<T(const T &)> > _mapOperations;

        std::vector<Operation> _pipeline;
    };
}

#endif //TEMPLATE_MY_STREAM_H
