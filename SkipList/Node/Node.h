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
                                                                           key_int64(static_cast<int64_t>(key)),
                                                                           self_index(self_index) {
        }

        explicit Node(): next_index(0), prev_index(0), key(), value(), key_int64(0), self_index(0) {
        }

        void setLevel(int level) {
            next_index = ds::Vector<int>(level, NODE_INVALID_INDEX);
            prev_index = ds::Vector<int>(level, NODE_INVALID_INDEX);
        }

        ~Node() = default;

        Node &operator=(const Node &other) {
            if (this == &other) {
                return *this;
            }
            next_index = other.next_index;
            prev_index = other.prev_index;
            key = other.key;
            value = other.value;
            key_int64 = other.key_int64;
            self_index = other.self_index;
            return *this;
        }

        template<typename K, typename V>
        friend class SkipList;
    };
}

#endif //SKIPLIST_NODE_H
