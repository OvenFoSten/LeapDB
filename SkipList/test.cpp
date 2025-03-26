#include <iostream>
#include"SkipList.h"
#include<random>
#include<chrono>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    long long terminate_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
};

int64_t calc_score(const int &key) {
    return static_cast<int64_t>(key);
}

int findV(std::vector<int> &v, int key) {
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == key) {
            return i;
        }
    }
    return -1;
}


int main() {
    ds::SkipList<int, int> test(calc_score);
    test.insert(1, 1);
    test.insert(2, 2);
    test.insert(3, 3);
    test.insert(4, 4);
    test.showStructure();
    test.remove(test.find(1));
    test.showStructure();
    test.insert(5, 5);
    test.insert(6, 6);
    test.showStructure();
    std::cout << "Remove -1" << std::endl;
    std::cout << (test.find(-1).hasValue() ? "Success" : "Failed") << std::endl;
    std::cout << (test.remove(-1) ? "Success" : "Failed") << std::endl;
    test.showStructure();
    test.validateStructure();
    return 0;
}
