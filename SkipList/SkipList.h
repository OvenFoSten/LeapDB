//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include"Node/Node.h"
#include"Vector/Vector.h"
#include<random>
#include<vector>

#include<iostream>

namespace ds {
    namespace detail {
        constexpr int INITIAL_LEVEL_COUNT = 8;
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

        int64_t (*calc_score)(const KEY &);

    public:
        explicit SkipList(int64_t (*calc_score)(const KEY &)): current_level_count(detail::INITIAL_LEVEL_COUNT),
                                                               head_indexes(
                                                                   detail::INITIAL_LEVEL_COUNT, NODE_INVALID_INDEX),
                                                               nodes(0),
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
                auto new_index = nodes.getSize();
                nodes.pushBack(ds::Node<KEY, VALUE>(current_level_count, new_index, key, value, score));

                auto current_find_level = current_level_count - 1;
                auto current_find_index = head_indexes[current_find_level];

                while (current_find_index == NODE_INVALID_INDEX) {
                    current_find_index = head_indexes[--current_find_level];
                }
                std::vector<size_t> prev_traces(current_level_count, NODE_INVALID_INDEX);
                std::vector<size_t> next_traces(current_level_count, NODE_INVALID_INDEX);
                while (true) {
                    //Browse current level till the end/next node has higher score
                    bool meet_bigger = false;
                    while (true) {
                        if (nodes[current_find_index].score > score) {
                            meet_bigger = true;
                            break;
                        }
                        if (nodes[current_find_index].next_index[current_find_level] == NODE_INVALID_INDEX) {
                            break;
                        }
                        current_find_index = nodes[current_find_index].next_index[current_find_level];
                    }
                    if (meet_bigger) {
                        next_traces[current_find_level] = current_find_index;
                        prev_traces[current_find_level] = nodes[current_find_index].prev_index[current_find_level];
                    } else {
                        next_traces[current_find_level] = NODE_INVALID_INDEX;
                        prev_traces[current_find_level] = current_find_index;
                    }
                    //If key already exists, update value
                    if (score == nodes[current_find_index].score) {
                        nodes[current_find_index].value = value;
                        break;
                    }
                    //If we reach bottom level, insert new node
                    if (current_find_level == detail::BOTTOM_LEVEL) {
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
                        break;
                    }
                    //If we haven't reach bottom level, go down one level& adjust to prev_traces
                    // std::cout<<"Level " << current_find_level << " ";
                    // std::cout << "Prev: " << (prev_traces[current_find_level] == NODE_INVALID_INDEX
                    //                               ? "INVALID"
                    //                               : std::to_string(nodes[prev_traces[current_find_level]].key));
                    if (prev_traces[current_find_level] != NODE_INVALID_INDEX) {
                        current_find_index = prev_traces[current_find_level];
                    }
                    //If prev_traces[current_find_level] == NODE_INVALID_INDEX, we need to go to head of this level
                    if (prev_traces[current_find_level] == NODE_INVALID_INDEX) {
                        current_find_index = head_indexes[current_find_level-1];
                    }
                    --current_find_level;
                    // std::cout << ",Jump to " << nodes[current_find_index].key << std::endl;
                }
            }
        }

        SkipList &operator=(const SkipList &other) = delete;

        SkipList(const SkipList &other) = delete;

        void showStructure() {
            for (int i = current_level_count - 1; i >= 0; --i) {
                std::cout << "Level " << i << ": " << std::flush;
                auto current_find_index = head_indexes[i];
                while (current_find_index != NODE_INVALID_INDEX) {
                    std::cout << nodes[current_find_index].key << " " << std::flush;
                    current_find_index = nodes[current_find_index].next_index[i];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        void validate() {
            std::vector<std::vector<size_t> > layers(current_level_count);
            for (int i = 0; i < current_level_count; ++i) {
                auto current_find_index = head_indexes[i];
                while (current_find_index != NODE_INVALID_INDEX) {
                    layers[i].push_back(nodes[current_find_index].key);
                    current_find_index = nodes[current_find_index].next_index[i];
                }
            }
            //Key exists above should exist below
            auto find = [&](int target, int layer)-> bool {
                for (int i = 0; i < layers[layer].size(); ++i) {
                    if (layers[layer][i] == target) {
                        return true;
                    }
                }
                return false;
            };
            for (int i = 0; i < current_level_count; ++i) {
                for (const auto &key: layers[i]) {
                    for (int j = 0; j < i; ++j) {
                        if (!find(key, j)) {
                            std::cout << "Key " << key << " exists in level " << i << " but not in level " << j <<
                                    std::endl;
                        }
                    }
                }
            }
        }
    };
} //ds

#endif //SKIPLIST_SKIPLIST_H
