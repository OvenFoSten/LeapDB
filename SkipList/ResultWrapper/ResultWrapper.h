//
// Created by OvenFoSten on 25-3-25.
//

#ifndef RESULTWRAPPER_H
#define RESULTWRAPPER_H

#include <cstddef>


namespace ds {
    template<typename VALUE>
    class ResultWrapper {
    private:
        VALUE *value_ptr{};
        size_t index{};

    public:
        ResultWrapper(VALUE *value_ptr, size_t index): value_ptr(value_ptr),
                                                       index(index) {
        }

        ResultWrapper() = delete;

        VALUE operator*() {
            return *value_ptr;
        }

        explicit operator VALUE() {
            return *value_ptr;
        }

        bool hasValue() {
            return value_ptr != nullptr;
        }


        template<typename K, typename V>
        friend class SkipList;
    };
}


#endif //RESULTWRAPPER_H
