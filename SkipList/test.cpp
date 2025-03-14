#include <iostream>
#include"SkipList.h"
#include<random>

int main() {
    ds::SkipList<int, int> test;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int SWAP_COUNT = 20;
    for (int i = 0; i < SWAP_COUNT; ++i) {
        int a = dis(gen);
        int b = dis(gen);
        std::swap(v[a], v[b]);
    }
    for (int i = 0; i < 10; ++i) {
        test.insert(v[i], v[i]);
    }

    std::cout << "Insert: ";
    for (const auto &i: v){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    test.visiable();

    return 0;
}
