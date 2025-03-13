//
// Created by OvenFoSten on 25-3-13.
//

#include "Node.h"

namespace ds {
    Node::Node(int level): data_index(0), next_index(level), prev_index(level) {
    }
} // ds
