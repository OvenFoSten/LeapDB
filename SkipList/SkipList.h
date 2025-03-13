//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include"Node/Node.h"
#include"Vector/Vector.h"

namespace {
    constexpr int INITIAL_LEVEL_COUNT = 4;
}

namespace ds {
    template<typename T>
    class SkipList {
    private:
        Node *head;
        Node *tail;
        ds::Vector<T> data;
        int current_level_count;

    public:
        SkipList(): head(nullptr), tail(nullptr), data(0), current_level_count(INITIAL_LEVEL_COUNT) {
        }

        SkipList& operator=(const SkipList& other)=delete;

        SkipList(const SkipList& other)=delete;
    };
} // ds

#endif //SKIPLIST_SKIPLIST_H
