//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

#include <cstddef>
#include "Vector/Vector.h"
#include <cstdint>

#include<mutex>

namespace ds {
    static constexpr int NODE_INVALID_INDEX = -1;

    template<typename KEY, typename VALUE>
    class Node {
    private:
        ds::Vector<int> next_index;
        ds::Vector<int> prev_index;
        KEY key;
        VALUE value;
        size_t self_index;
        int64_t score;

        std::mutex node_mutex;

    public:
        explicit Node(int level,
                      size_t self_index,
                      KEY key,
                      VALUE value,
                      int64_t score): next_index(level, NODE_INVALID_INDEX),
                                      prev_index(level, NODE_INVALID_INDEX),
                                      key(key),
                                      value(value),
                                      self_index(self_index),
                                      score(score) {
        }

        explicit Node(): next_index(0), prev_index(0), key(), value(), self_index(0), score(0) {
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
            self_index = other.self_index;
            score = other.score;
            return *this;
        }

        template<typename K, typename V>
        friend class SkipList;
    };
}

#endif //SKIPLIST_NODE_H
