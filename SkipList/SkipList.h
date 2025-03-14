//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include"Node/Node.h"
#include"Vector/Vector.h"
#include<random>
#include<stack>

namespace {
    constexpr int INITIAL_LEVEL_COUNT = 4;
    constexpr int BOTTOM_LEVEL = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
}

namespace ds {
    template<typename KEY, typename VALUE>
    class SkipList {
    private:
        Node<KEY, VALUE> *head;
        Node<KEY, VALUE> *tail;
        ds::Vector<Node<KEY, VALUE> > nodes;
        int current_level_count;

        Node<KEY, VALUE> *findOrCreateNode(const KEY &key, const VALUE &value) {
            Node<KEY, VALUE> *current = head;
            auto find_level = current_level_count - 1;
            auto key_int64 = reinterpret_cast<int64_t>(key);

            while (true) {
                if (key_int64 == current->key_int64) {
                    return current;
                }
                auto jump_index = NODE_INVALID_INDEX;
                if (key_int64 < current->key_int64) {
                    jump_index = current->prev_index[find_level];
                } else if (key_int64 > current->key_int64) {
                    jump_index = current->next_index[find_level];
                }
                if (jump_index == NODE_INVALID_INDEX) {
                    if (find_level == BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        nodes.push_back(Node<KEY, VALUE>(current_level_count, new_index, key, value));
                        if (current == head) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = dis(gen);
                                if (!factor)break;
                                head->prev_index[i] = new_index;
                                nodes[new_index].next_index[i] = head->self_index;
                            }
                            head = &nodes[new_index];
                        }
                        if (current == tail) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = dis(gen);
                                if (!factor)break;
                                tail->next_index[i] = new_index;
                                nodes[new_index].prev_index[i] = tail->self_index;
                            }
                            tail = &nodes[new_index];
                        }
                        return &nodes[new_index];
                    }
                    find_level--;
                    continue;
                }

                if (nodes[jump_index].key_int64 > key_int64 && current->key_int64 < key_int64) {
                    if (find_level == BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        nodes.push_back(Node<KEY, VALUE>(current_level_count, new_index, key, value));
                        for (int i = 0; i < current_level_count; ++i) {
                            auto factor = dis(gen);
                            if (!factor)break;
                            nodes[jump_index].next_index[i] = new_index;
                            nodes[new_index].prev_index[i] = jump_index;

                            nodes[new_index].next_index[i] = current->self_index;
                            current->prev_index[i] = new_index;
                        }
                        return &nodes[new_index];
                    }else {
                        find_level--;
                        continue;
                    }
                }

                current = nodes[jump_index];
            }
        }

    public:
        SkipList(): head(nullptr),
                    tail(nullptr),
                    nodes(0),
                    current_level_count(INITIAL_LEVEL_COUNT) {
        }

        SkipList &operator=(const SkipList &other) = delete;

        SkipList(const SkipList &other) = delete;

        void insert(KEY key, VALUE value) {
            //First Handle the case when the list is empty
            if (head == nullptr && tail == nullptr) {
                nodes.push_back(Node<KEY, VALUE>(current_level_count, key, value));
                head = &nodes[0];
                tail = &nodes[0];
                return;
            }
        }
    };
} // ds

#endif //SKIPLIST_SKIPLIST_H
