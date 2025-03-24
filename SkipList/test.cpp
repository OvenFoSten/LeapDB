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
    const int DATA_SIZE = 100000;
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
    std::cout<<"Building skiplist..."<<std::endl;
    Timer t;
    for (int i = 0; i < DATA_SIZE; ++i) {
        // std::cout<<"Insert "<<v[i]<<"..."<<std::endl;
        test.insert(v[i], v[i]);
        // test.showStructure();
    }
    std::cout << t.terminate_ms()<<"ms"<<std::endl;
    std::priority_queue<int> opponent;
    t.reset();
    for (int i = 0; i < DATA_SIZE; ++i) {
        opponent.push(v[i]);
    }
    std::cout << t.terminate_ms()<<"ms"<<std::endl;
    std::cout<<"Measuring random time cost..."<<std::endl;
    t.reset();
    for (int i = 0; i < DATA_SIZE; ++i) {
        int a = ds::detail::raise_dis(ds::detail::raise_gen);
    }
    std::cout << t.terminate_ms()<<"ms"<<std::endl;
    std::cout<<"Validating..."<<std::endl;
    test.validate();
    return 0;
}
