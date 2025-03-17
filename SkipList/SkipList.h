//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include"Node/Node.h"
#include"Vector/Vector.h"
#include<random>
#include<stack>

#include<iostream>

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

        int64_t (*calc_score)(const KEY &);

        Node<KEY, VALUE> *findOrCreateNode(const KEY &key, const VALUE &value, int64_t score) {
            auto *current = head;
            auto find_level = current_level_count - 1;

            while (true) {
                if (score == current->score) {
                    return current;
                }
                auto jump_index = NODE_INVALID_INDEX;
                if (score < current->score) {
                    jump_index = current->prev_index[find_level];
                } else if (score > current->score) {
                    jump_index = current->next_index[find_level];
                }
                if (jump_index == NODE_INVALID_INDEX) {
                    if (find_level == BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        nodes.pushBack(Node<KEY, VALUE>(current_level_count, new_index, key, value, score));
                        if (score < current->score) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = dis(gen) || !i;
                                if (!factor)break;
                                head->prev_index[i] = new_index;
                                nodes[new_index].next_index[i] = head->self_index;
                            }
                            head = &nodes[new_index];
                        }
                        if (score > current->score) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = dis(gen) || !i;
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

                if (nodes[jump_index].score > score && current->score < score) {
                    if (find_level == BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        nodes.pushBack(Node<KEY, VALUE>(current_level_count, new_index, key, value, score));
                        for (int i = 0; i < current_level_count; ++i) {
                            auto factor = dis(gen) || !i;
                            if (!factor)break;
                            nodes[jump_index].prev_index[i] = new_index;
                            nodes[new_index].next_index[i] = jump_index;

                            nodes[new_index].prev_index[i] = current->self_index;
                            current->next_index[i] = new_index;
                        }
                        return &nodes[new_index];
                    } else {
                        find_level--;
                        continue;
                    }
                }

                current = &nodes[jump_index];
            }
        }

    public:
        SkipList(int64_t (*calc_score)(const KEY &)): head(nullptr),
                                                      tail(nullptr),
                                                      nodes(0),
                                                      current_level_count(INITIAL_LEVEL_COUNT),
                                                      calc_score(calc_score) {
        }

        SkipList &operator=(const SkipList &other) = delete;

        SkipList(const SkipList &other) = delete;

        void insert(KEY key, VALUE value) {
            auto score = calc_score(key);
            //First Handle the case when the list is empty
            if (head == nullptr && tail == nullptr) {
                nodes.pushBack(Node<KEY, VALUE>(current_level_count, 0, key, value, score));
                head = nodes.begin();
                tail = nodes.begin();
                return;
            }
            findOrCreateNode(key, value, score);
        }

        void showStructure() {
            std::vector<std::vector<int> > v(current_level_count);
            auto *current = head;
            while (true) {
                for (int i = 0; i < current_level_count; ++i) {
                    v[i].push_back(current->next_index[i]);
                }
                if (current->next_index[BOTTOM_LEVEL] == NODE_INVALID_INDEX) {
                    break;
                }
                current = &nodes[current->next_index[BOTTOM_LEVEL]];
            }

            std::cout << "SkipList Structure:" << std::endl;
            for (int i = current_level_count - 1; i >= BOTTOM_LEVEL; --i) {
                std::cout << "Level " << i << ": ";
                if (i == BOTTOM_LEVEL)std::cout << head->key << " ";
                else std::cout << "  ";
                for (const auto index: v[i]) {
                    if (index == NODE_INVALID_INDEX) {
                        std::cout << "  ";
                    } else {
                        std::cout << nodes[index].key << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
    };
} // ds

#endif //SKIPLIST_SKIPLIST_H
