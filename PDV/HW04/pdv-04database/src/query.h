#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t&)>;


template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);


template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    std::atomic<bool> ret {true};

    #pragma omp parallel for
    for(unsigned i = 0; i < predicates.size(); i++){
        if(!ret) continue;

        bool is_ok = false;
        auto &pred = predicates[i];
        for(unsigned j = 0; j < data_table.size(); j++){
            auto &row = data_table[j];
            bool is_satisfied = pred(row);
            if(is_satisfied){
                is_ok = true;
                break;
            }
        }
        if(!is_ok) {
            ret = false;
            #pragma omp cancel for
        }
    }
    return ret;
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    std::atomic<bool> ret = {false};

    #pragma omp parallel for
    for (unsigned i = 0; i < data_table.size(); i++){
        auto &row = data_table[i];
        for(unsigned j = 0; j < predicates.size(); j++){
            auto &pred = predicates[j];
            bool is_ok = pred(row);
            if(is_ok){
                ret = true;
                break;
            }
        }
        if(ret) {
            #pragma omp cancel for
        }
    }
    return ret;
}
