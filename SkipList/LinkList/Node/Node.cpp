//
// Created by OvenFoSten on 25-3-13.
//

#include "Node.h"

namespace ds {
    Node::Node() : data_index(0) {
        next_index = Vector<std::size_t>(INITIAL_LEVEL);
        prev_index = Vector<std::size_t>(INITIAL_LEVEL);
    }
} // ds
