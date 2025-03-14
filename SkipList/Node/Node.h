//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

#include <cstddef>
#include "Vector/Vector.h"
#include <cstdint>

namespace ds {
    static constexpr int NODE_INVALID_INDEX = -1;

    template<typename KEY, typename VALUE>
    class Node {
    private:
        ds::Vector<int> next_index;
        ds::Vector<int> prev_index;
        KEY key;
        VALUE value;
        int64_t key_int64;
        size_t self_index;

    public:
        explicit Node(int level, size_t self_index, KEY key, VALUE value): next_index(level, NODE_INVALID_INDEX),
                                                                           prev_index(level, NODE_INVALID_INDEX),
                                                                           key(key),
                                                                           value(value),
                                                                           key_int64(reinterpret_cast<int64_t>(key)),
                                                                           self_index(self_index) {
        }

        ~Node() = default;

        template<typename T>
        friend class SkipList;
    };
}

#endif //SKIPLIST_NODE_H
