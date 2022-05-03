//
// Created by balik on 5/3/2022.
//

#ifndef TEMPLATE_MY_STREAM_H
#define TEMPLATE_MY_STREAM_H

namespace my_std {
    template<typename T, template<typename...> typename Container = std::vector>
    class stream {
    private:
        Container<T> container;
    public:
        // todo:
    };
}

#endif //TEMPLATE_MY_STREAM_H
