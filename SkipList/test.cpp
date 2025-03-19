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
    const int DATA_SIZE = 10000;
    const int SWAP_COUNT = DATA_SIZE * 2;
    ds::SkipList<int, int> test(calc_score);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, DATA_SIZE - 1);
    std::vector<int> v(DATA_SIZE);
    for (int i = 0; i < DATA_SIZE; ++i) {
        v[i] = i;
    }
    for (int i = 0; i < SWAP_COUNT; ++i) {
        int a = dis(gen);
        int b = dis(gen);
        std::swap(v[a], v[b]);
    }
    for (int i = 0; i < DATA_SIZE; ++i) {
        test.insert(v[i], v[i]);
    }

    // std::cout << "Insert: ";
    for (const auto &i: v) {
        // std::cout << i << " ";
    }
    // std::cout << std::endl;

    // test.showStructure();

    Timer t;
    bool pass = true;
    for (const auto &i: v) {
        bool find = test.find(i);
        if (!find) {
            // std::cout << "\033[1;31mFailed to find " << i << "\033[0m" << std::endl;
            pass = false;
        }
    }
    std::cout << "Find: " << t.terminate_ms() << "ms" << std::endl;
    std::cout << (pass ? "\033[1;32mPass\033[0m" : "\033[1;31mFail\033[0m") << std::endl;
    t.reset();
    for (const auto &i: v) {
        int a = findV(v, i);
        if (a == -1) {
            std::cout << "\033[1;31mFailed to find " << i << "\033[0m" << std::endl;
            pass = false;
        }
    }
    std::cout << "Find: " << t.terminate_ms() << "ms" << std::endl;
    return 0;
}
