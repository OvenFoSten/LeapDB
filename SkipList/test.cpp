#include <iostream>
#include"SkipList.h"
#include<chrono>
#include<unordered_map>
#include<vector>
#include<utility>
#include<random>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    [[nodiscard]] long long terminate_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
};

int64_t calc_score(const int &key) {
    return static_cast<int64_t>(key);
}

//Use <int,int> as key-value pair
int main() {
    //Configuration
    const int DATA_SIZE = 100000;
    const int TEST_RUNS = 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> value_dis(INT_MIN, INT_MAX);
    std::uniform_int_distribution<> index_dis(0, DATA_SIZE - 1);
    std::uniform_int_distribution<> bool_dis(0, 1);
    ds::SkipList<int, int> skiplist(calc_score);

    double avg_insert_time = 0;
    double avg_find_time = 0;
    double avg_delete_time = 0;

    bool all_pass = true;
    for (int i = 0; i < TEST_RUNS; ++i) {
        bool pass = true;
        std::vector<std::pair<int, int> > data(DATA_SIZE);
        std::unordered_map<int, int> compare;
        compare.reserve(DATA_SIZE);
        std::cout << "Generating data..." << std::endl;
        for (auto &pair: data) {
            pair.first = value_dis(gen);
            pair.second = value_dis(gen);
            //Simulate a correct skiplist
            compare[pair.first] = pair.second;
        }
        std::cout << "Data generated!" << std::endl;
        std::cout << "Test Insert..." << std::endl;
        Timer timer;
        for (const auto &[fst, snd]: data) {
            skiplist.insert(fst, snd);
        }
        auto insert_time = timer.terminate_ms();
        avg_insert_time += static_cast<double>(insert_time) / TEST_RUNS;
        std::cout << "Insert " << DATA_SIZE << " items took " << insert_time << "ms" << std::endl;
        std::cout << "Check Structure..." << std::endl;
        if (!skiplist.validateStructure()) {
            std::cout << "Error! Structure is broken!" << std::endl;
            pass = false;
        } else {
            std::cout << "Structure is valid!" << std::endl;
        }
        std::cout << "Test Find..." << std::endl;
        timer.reset();
        for (const auto &[fst, snd]: compare) {
            auto result = skiplist.find(fst);
            if (!result.hasValue()) {
                std::cout << "Error! Key: " << fst << " not found!" << std::endl;
                pass = false;
                continue;
            }
            if (static_cast<int>(result) != snd) {
                std::cout << "Error! Key: " << fst << " has value: " << static_cast<int>(result) << " but expected: "
                        << snd << std::endl;
                pass = false;
            }
        }
        auto find_time = timer.terminate_ms();
        avg_find_time += static_cast<double>(find_time) / TEST_RUNS;
        std::cout << "Find " << DATA_SIZE << " items took " << find_time << "ms" << std::endl;
        std::cout << "Test Delete..." << std::endl;
        timer.reset();
        for (const auto &[fst, snd]: data) {
            skiplist.remove(fst);
        }
        auto delete_time = timer.terminate_ms();
        avg_delete_time += static_cast<double>(delete_time) / TEST_RUNS;
        std::cout << "Delete " << DATA_SIZE << " items took " << delete_time << "ms" << std::endl;
        std::cout << "Check Empty..." << std::endl;
        for (const auto &[fst, snd]: data) {
            auto result = skiplist.find(fst);
            if (result.hasValue()) {
                std::cout << "Error! Key: " << fst << " not deleted!" << std::endl;
                pass = false;
            }
        }
        std::cout << "Check Empty complete!" << std::endl;
        std::cout << "Check Structure..." << std::endl;
        if (!skiplist.validateStructure()) {
            std::cout << "Error! Structure is broken!" << std::endl;
            pass = false;
        } else {
            std::cout << "Structure is valid!" << std::endl;
        }
        std::cout << "Check Structure complete!" << std::endl;
        std::cout << "Test " << i + 1 << (pass ? "\033[32m Passed \033[0m" : "\033[31m Failed \033[0m") << std::endl;
        all_pass = all_pass && pass;
    }
    std::cout << "Extra Test..." << std::endl;
    bool pass = true;
    std::vector<std::pair<int, int> > data(DATA_SIZE);
    std::unordered_map<int, int> compare;
    compare.reserve(DATA_SIZE);
    std::cout << "Generating data..." << std::endl;
    for (auto &[fst, snd]: data) {
        fst = value_dis(gen);
        snd = value_dis(gen);
    }
    std::cout << "Data generated!" << std::endl;
    for (const auto &d: data) {
        if (bool_dis(gen)) {
            skiplist.insert(d.first, d.second);
            compare[d.first] = d.second;
        } else {
            skiplist.remove(d.first);
            compare.erase(d.first);
        }
    }
    for (const auto &d: data) {
        if (bool_dis(gen)) {
            skiplist.insert(d.first, d.second);
            compare[d.first] = d.second;
        } else {
            skiplist.remove(d.first);
            compare.erase(d.first);
        }
    }
    std::cout << "Check Data correctness..." << std::endl;
    for (const auto &d: data) {
        bool compare_find = compare.find(d.first) != compare.end();
        auto compare_result = compare_find ? compare[d.first] : 0;
        auto result = skiplist.find(d.first);
        bool skiplist_find = result.hasValue();
        if (compare_find != skiplist_find) {
            std::cout << "Error! Compare" << (compare_find ? " has" : " don't have") << " key: " << d.first <<
                    " but SkipList" << (skiplist_find ? " has" : " don't have") << " key: " << d.first << std::endl;
            pass = false;
        } else if (compare_find && skiplist_find) {
            if (static_cast<int>(result) != compare_result) {
                std::cout << "Error! Key: " << d.first << " has value: " << static_cast<int>(result) <<
                        " but expected: "
                        << compare_result << std::endl;
                pass = false;
            }
        }
    }
    if (!skiplist.validateStructure()) {
        std::cout << "Error! Structure is broken!" << std::endl;
        pass = false;
    } else {
        std::cout << "Structure is valid!" << std::endl;
    }
    all_pass = all_pass && pass;

    std::cout << "========================" << std::endl;
    std::cout << "All tests " << (all_pass ? "\033[32m Passed \033[0m" : "\033[31m Failed \033[0m") << std::endl;
    std::cout << "Average Insert time: " << avg_insert_time << "ms" << std::endl;
    std::cout << "Average Find time: " << avg_find_time << "ms" << std::endl;
    std::cout << "Average Delete time: " << avg_delete_time << "ms" << std::endl;
    return 0;
}
