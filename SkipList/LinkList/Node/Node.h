//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

#include <cstddef>
#include "Vector/Vector.h"

namespace {
    //Configurations
    const int INITIAL_LEVEL = 4;
}

namespace ds {
    class Node {
    private:
        std::size_t data_index;
        ds::Vector<std::size_t> next_index;
        ds::Vector<std::size_t> prev_index;
    public:
        Node();

        ~Node() = default;
    };
}

#endif //SKIPLIST_NODE_H
