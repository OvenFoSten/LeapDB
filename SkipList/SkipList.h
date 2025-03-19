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

namespace ds {
    namespace detail {
        constexpr int INITIAL_LEVEL_COUNT = 4;
        constexpr int BOTTOM_LEVEL = 0;
        inline std::random_device rd;
        inline std::mt19937 gen(rd());
        inline std::uniform_int_distribution<> dis(0, 1);
    }

    template<typename KEY, typename VALUE>
    class SkipList {
    private:
        size_t head_index;
        size_t tail_index;
        ds::Vector<Node<KEY, VALUE> > nodes;
        int current_level_count;

        int64_t (*calc_score)(const KEY &);

        void findOrCreateNode(const KEY &key, const VALUE &value, int64_t score) {
            auto current_index = head_index;
            auto find_level = current_level_count - 1;

            while (true) {
                if (score == nodes[current_index].score) {
                    return;
                }
                auto jump_index = NODE_INVALID_INDEX;
                if (score < nodes[current_index].score) {
                    jump_index = nodes[current_index].prev_index[find_level];
                } else if (score > nodes[current_index].score) {
                    jump_index = nodes[current_index].next_index[find_level];
                }
                if (jump_index == NODE_INVALID_INDEX) {
                    if (find_level == detail::BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        auto new_node = Node<KEY, VALUE>(current_level_count, new_index, key, value, score);
                        nodes.pushBack(new_node);
                        if (score < nodes[current_index].score) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = detail::dis(detail::gen) || !i;
                                if (!factor)break;
                                nodes[head_index].prev_index[i] = new_index;
                                nodes[new_index].next_index[i] = nodes[head_index].self_index;
                            }
                            head_index = new_index;
                        }
                        if (score > nodes[current_index].score) {
                            for (int i = 0; i < current_level_count; ++i) {
                                auto factor = detail::dis(detail::gen) || !i;
                                if (!factor)break;
                                nodes[tail_index].next_index[i] = new_index;
                                nodes[new_index].prev_index[i] = nodes[tail_index].self_index;
                            }
                            tail_index = new_index;
                        }
                        return;
                    }
                    find_level--;
                    continue;
                }

                if (nodes[jump_index].score > score && nodes[current_index].score < score) {
                    if (find_level == detail::BOTTOM_LEVEL) {
                        auto new_index = nodes.getSize();
                        nodes.pushBack(Node<KEY, VALUE>(current_level_count, new_index, key, value, score));
                        for (int i = 0; i < current_level_count; ++i) {
                            auto factor = detail::dis(detail::gen) || !i;
                            if (!factor)break;
                            nodes[jump_index].prev_index[i] = new_index;
                            nodes[new_index].next_index[i] = jump_index;

                            nodes[new_index].prev_index[i] = nodes[current_index].self_index;
                            nodes[current_index].next_index[i] = new_index;
                        }
                        return;
                    }
                    find_level--;
                    continue;
                }
                current_index = jump_index;
            }
        }

    public:
        explicit SkipList(int64_t (*calc_score)(const KEY &)): head_index(NODE_INVALID_INDEX),
                                                               tail_index(NODE_INVALID_INDEX),
                                                               nodes(0),
                                                               current_level_count(detail::INITIAL_LEVEL_COUNT),
                                                               calc_score(calc_score) {
        }

        SkipList &operator=(const SkipList &other) = delete;

        SkipList(const SkipList &other) = delete;

        void insert(KEY key, VALUE value) {
            // std::cout << "Inserting " << key << std::endl;
            auto score = calc_score(key);
            //First Handle the case when the list is empty
            if (head_index == NODE_INVALID_INDEX && tail_index == NODE_INVALID_INDEX) {
                nodes.pushBack(Node<KEY, VALUE>(current_level_count, 0, key, value, score));
                head_index = 0;
                tail_index = 0;
                return;
            }
            findOrCreateNode(key, value, score);
        }

        void showStructure() {
            std::vector<std::vector<int> > v(current_level_count);
            auto current_index = head_index;
            while (true) {
                for (int i = 0; i < current_level_count; ++i) {
                    v[i].push_back(nodes[current_index].next_index[i]);
                }
                if (nodes[current_index].next_index[detail::BOTTOM_LEVEL] == NODE_INVALID_INDEX) {
                    break;
                }
                current_index = nodes[current_index].next_index[detail::BOTTOM_LEVEL];
            }

            std::cout << "SkipList Structure:" << std::endl;
            for (int i = current_level_count - 1; i >= detail::BOTTOM_LEVEL; --i) {
                std::cout << "Level " << i << ": ";
                if (i == detail::BOTTOM_LEVEL)std::cout << nodes[head_index].key << " ";
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

        VALUE *find(const KEY &key) {
            auto current_index = head_index;
            auto find_level = current_level_count - 1;
            auto score = calc_score(key);

            while (true) {
                if (score == nodes[current_index].score) {
                    return &(nodes[current_index].value);
                }
                auto jump_index = NODE_INVALID_INDEX;
                if (score < nodes[current_index].score) {
                    jump_index = nodes[current_index].prev_index[find_level];
                } else if (score > nodes[current_index].score) {
                    jump_index = nodes[current_index].next_index[find_level];
                }
                if (jump_index == NODE_INVALID_INDEX) {
                    if (find_level == detail::BOTTOM_LEVEL) {
                        return nullptr;
                    }
                    find_level--;
                    continue;
                }

                if (nodes[jump_index].score > score && nodes[current_index].score < score) {
                    if (find_level == detail::BOTTOM_LEVEL) {
                        return nullptr;
                    }
                    find_level--;
                    continue;
                }
                current_index = jump_index;
            }
        }
    };
} // ds

#endif //SKIPLIST_SKIPLIST_H
