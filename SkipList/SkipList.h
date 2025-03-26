//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include"Node/Node.h"
#include"Vector/Vector.h"
#include"ResultWrapper/ResultWrapper.h"
#include<random>
#include<vector>

#include<iostream>
#include <unordered_set>

namespace ds {
    namespace detail {
        constexpr int INITIAL_LEVEL_COUNT = 64;
        constexpr int BOTTOM_LEVEL = 0;
        inline std::random_device raise_rd;
        inline std::mt19937 raise_gen(raise_rd());
        inline std::uniform_int_distribution<> raise_dis(0, 1);
    }

    template<typename KEY, typename VALUE>
    class SkipList {
    private:
        ds::Vector<size_t> head_indexes;
        ds::Vector<Node<KEY, VALUE> > nodes;
        int current_level_count;
        ds::Vector<size_t> free_indexes;

        int64_t (*calc_score)(const KEY &);

    public:
        explicit SkipList(int64_t (*calc_score)(const KEY &)): current_level_count(detail::INITIAL_LEVEL_COUNT),
                                                               head_indexes(
                                                                   detail::INITIAL_LEVEL_COUNT, NODE_INVALID_INDEX),
                                                               nodes(0),
                                                               free_indexes(0),
                                                               calc_score(calc_score) {
        }

        void insert(const KEY &key, const VALUE &value) {
            auto score = calc_score(key);
            if (nodes.empty()) {
                nodes.pushBack(ds::Node<KEY, VALUE>(current_level_count, 0, key, value, score));
                for (int i = 0; i < current_level_count; ++i) {
                    head_indexes[i] = 0;
                    if (detail::raise_dis(detail::raise_gen)) {
                        break;
                    }
                }
            } else {
                auto current_find_level = current_level_count - 1;
                auto current_find_index = head_indexes[current_find_level];

                while (current_find_index == NODE_INVALID_INDEX && current_find_level > detail::BOTTOM_LEVEL) {
                    current_find_index = head_indexes[--current_find_level];
                }
                std::vector<size_t> prev_traces(current_level_count, NODE_INVALID_INDEX);
                std::vector<size_t> next_traces(current_level_count, NODE_INVALID_INDEX);
                while (true) {
                    //Browse current level till the end/next node has higher score
                    while (current_find_index != NODE_INVALID_INDEX && score > nodes[current_find_index].score) {
                        if (nodes[current_find_index].next_index[current_find_level] == NODE_INVALID_INDEX) {
                            prev_traces[current_find_level] = current_find_index;
                        }
                        current_find_index = nodes[current_find_index].next_index[current_find_level];
                    }
                    // if (meet_bigger) {
                    next_traces[current_find_level] = current_find_index;
                    if (current_find_index != NODE_INVALID_INDEX) {
                        prev_traces[current_find_level] = nodes[current_find_index].prev_index[current_find_level];

                        //If key already exists, update value
                        if (score == nodes[current_find_index].score) {
                            nodes[current_find_index].value = value;
                            return;
                        }
                    }
                    //If we reach bottom level, insert new node
                    if (current_find_level == detail::BOTTOM_LEVEL) {
                        size_t new_index = NODE_INVALID_INDEX;
                        if (!free_indexes.empty()) {
                            new_index = free_indexes[free_indexes.getSize() - 1];
                            nodes[new_index] = ds::Node<KEY, VALUE>(current_level_count, new_index, key, value, score);
                            free_indexes.popBack();
                        } else {
                            new_index = nodes.getSize();
                            nodes.pushBack(ds::Node<KEY, VALUE>(current_level_count, new_index, key, value, score));
                        }
                        for (int i = 0; i < current_level_count; ++i) {
                            if (head_indexes[i] == NODE_INVALID_INDEX) {
                                head_indexes[i] = new_index;
                            }
                            nodes[new_index].next_index[i] = next_traces[i];
                            nodes[new_index].prev_index[i] = prev_traces[i];

                            if (prev_traces[i] != NODE_INVALID_INDEX) {
                                nodes[prev_traces[i]].next_index[i] = new_index;
                            } else {
                                head_indexes[i] = new_index;
                            }

                            if (next_traces[i] != NODE_INVALID_INDEX) {
                                nodes[next_traces[i]].prev_index[i] = new_index;
                            }

                            if (detail::raise_dis(detail::raise_gen)) {
                                break;
                            }
                        }
                        return;
                    }
                    if (prev_traces[current_find_level] != NODE_INVALID_INDEX) {
                        current_find_index = prev_traces[current_find_level];
                    }
                    if (prev_traces[current_find_level] == NODE_INVALID_INDEX) {
                        current_find_index = head_indexes[current_find_level - 1];
                    }
                    --current_find_level;
                }
            }
        }

        ResultWrapper<VALUE> find(const KEY &key) {
            if (nodes.empty()) {
                return ResultWrapper<VALUE>{nullptr, 0};
            }
            auto score = calc_score(key);
            auto current_find_level = current_level_count - 1;
            auto current_find_index = head_indexes[current_find_level];

            while (current_find_index == NODE_INVALID_INDEX && current_find_level > detail::BOTTOM_LEVEL) {
                current_find_index = head_indexes[--current_find_level];
            }
            while (true) {
                while (current_find_index != NODE_INVALID_INDEX && score > nodes[current_find_index].score) {
                    current_find_index = nodes[current_find_index].next_index[current_find_level];
                }
                if (current_find_index != NODE_INVALID_INDEX && score == nodes[current_find_index].score) {
                    return ResultWrapper<VALUE>{&nodes[current_find_index].value, current_find_index};
                }
                if (current_find_level == detail::BOTTOM_LEVEL) {
                    return ResultWrapper<VALUE>{nullptr, 0};
                }
                if (current_find_index != NODE_INVALID_INDEX) {
                    current_find_index = nodes[current_find_index].prev_index[current_find_level];
                }
                if (current_find_index == NODE_INVALID_INDEX) {
                    current_find_index = head_indexes[current_find_level - 1];
                }
                --current_find_level;
            }
        }

        bool remove(const KEY &key) {
            auto result = find(key);
            if (result.hasValue()) {
                return remove(result);
            }
            return false;
        }

        bool remove(const ResultWrapper<VALUE> &result) {
            auto current_find_index = result.index;
            if (current_find_index >= nodes.getSize()) {
                return false;
            }
            if (current_find_index == NODE_INVALID_INDEX) {
                return false;
            }
            for (int i = 0; i < current_level_count; ++i) {
                char bitmask = 0B00; //prev next,valid->1 invalid->0
                auto &current_node = nodes[current_find_index];
                if (current_node.prev_index[i] != NODE_INVALID_INDEX) {
                    bitmask |= 0B10;
                }
                if (current_node.next_index[i] != NODE_INVALID_INDEX) {
                    bitmask |= 0B01;
                }
                if (bitmask == 0B00) {
                    /*Two conditions:
                     *1. The node is the only node in the list&&self index is head index
                     *-> mark,collect
                     *2. The node isn't exist in current level(and levels above)
                     *break
                     */
                    if (current_node.self_index == head_indexes[i]) {
                        head_indexes[i] = NODE_INVALID_INDEX;
                    } else {
                        break;
                    }
                }
                if (bitmask == 0B01) {
                    //Self is head index&&next is valid
                    head_indexes[i] = current_node.next_index[i];
                    nodes[current_node.next_index[i]].prev_index[i] = NODE_INVALID_INDEX;
                }
                if (bitmask == 0B10) {
                    //Self is tail index&&prev is valid
                    nodes[current_node.prev_index[i]].next_index[i] = NODE_INVALID_INDEX;
                }
                if (bitmask == 0B11) {
                    //Normal case
                    nodes[current_node.prev_index[i]].next_index[i] = current_node.next_index[i];
                    nodes[current_node.next_index[i]].prev_index[i] = current_node.prev_index[i];
                }
            }
            //Collect
            free_indexes.pushBack(current_find_index);

            return true;
        }

        SkipList &operator=(const SkipList &other) = delete;

        SkipList(const SkipList &other) = delete;

        void showStructure() {
            for (int i = current_level_count - 1; i >= 0; --i) {
                auto current_find_index = head_indexes[i];
                if (current_find_index == NODE_INVALID_INDEX)continue;
                std::cout << "Level " << i << ": " << std::flush;
                while (current_find_index != NODE_INVALID_INDEX) {
                    std::cout << nodes[current_find_index].key << " " << std::flush;
                    current_find_index = nodes[current_find_index].next_index[i];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        bool validateStructure() {
            bool pass = true;
            std::cout << "Validating structure..." << std::endl;
            std::vector<std::unordered_set<int64_t> > check(current_level_count);
            for (int current_level = 0; current_level < current_level_count; ++current_level) {
                auto current_index = head_indexes[current_level];
                size_t prev_index = NODE_INVALID_INDEX;
                while (current_index != NODE_INVALID_INDEX) {
                    if (prev_index != NODE_INVALID_INDEX) {
                        if (nodes[current_index].score <= nodes[prev_index].score) {
                            std::cout << "Error! Key: " << nodes[prev_index].key << " is bigger than Key: " << nodes[
                                current_index].key << std::endl;
                            pass = false;
                        }
                    }
                    prev_index = current_index;
                    check[current_level].insert(nodes[current_index].score);
                    current_index = nodes[current_index].next_index[current_level];
                }
                auto current_layer_size = check[current_level].size();
                current_index = prev_index;
                size_t count = 0;
                while (current_index != NODE_INVALID_INDEX) {
                    count++;
                    current_index = nodes[current_index].prev_index[current_level];
                }
                if (current_layer_size != count) {
                    std::cout << "Error! Level " << current_level << " structure is broken! " << std::endl;
                    pass = false;
                }
            }

            for (int current_level = current_level_count - 1; current_level >= 0; --current_level) {
                auto &current_layer = check[current_level];
                for (const auto &score: current_layer) {
                    for (int i = current_level; i >= 0; --i) {
                        if (!check[i].contains(score)) {
                            std::cout << "Error! Score: " << score << " is miss in level " << i << std::endl;
                            pass = false;
                        }
                    }
                }
            }
            std::cout << "Validation complete!" << std::endl;

            return pass;
        }
    };
} //ds

#endif //SKIPLIST_SKIPLIST_H
