#include "bfs.h"

#include <unordered_set>
#include <atomic>
#include <limits>
#include <omp.h>
#include <iostream>

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi cesty v grafu.
// V teto metode nemusite prilis optimalizovat pametove naroky, a vhodnym algo-
// ritmem tak muze byt napriklad pouziti prohledavani do sirky (breadth-first
// search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi cesty.

#define MAX_THREADS 8

state_ptr bfs(state_ptr root) {
    if (root == nullptr) return nullptr;
    if (root->is_goal()) return root;

    int threads_num = omp_get_max_threads();
    threads_num = std::min(MAX_THREADS, threads_num);

    state_ptr ret = nullptr;
    std::atomic<bool> found_goal = false;
    std::atomic<unsigned long long > goal_id;
    std::vector<std::vector<state_ptr>> queue(2);
    std::vector<std::vector<std::vector<state_ptr>>> local_queue(2, std::vector<std::vector<state_ptr>>(threads_num));
    std::unordered_set<unsigned long long> visited;

    int level = 0;
    queue[level].push_back(root);
    while (true) {
        #pragma omp parallel for schedule(dynamic, 4) num_threads(threads_num) shared(ret, found_goal, goal_id)
        for (size_t i = 0; i < queue[level].size(); i++){
            state_ptr current = queue[level][i];
            if (found_goal && goal_id <= current->get_identifier()) continue;
            int thread_id = omp_get_thread_num();
            if (visited.find(current->get_identifier()) == visited.end()){
                #pragma omp critical
                {
                    if (visited.find(current->get_identifier()) == visited.end()){
                        visited.insert(current->get_identifier());
                        if(current->is_goal()){
                            found_goal = true;
                            ret = current;
                            goal_id = current->get_identifier();
                        }
                    }
                }
                if (!found_goal){
                    for(const auto &next : current->next_states()){
                        local_queue[(level+1)%2][thread_id].push_back(next);
                    }
                }
            }
        }
        if(found_goal) break;
        level = (level+1)%2;

        for(int i = 0; i < threads_num; i++){
            queue[level].insert(queue[level].end(), local_queue[level][i].begin(), local_queue[level][i].end());
            local_queue[level][i].clear();
        }
        queue[(level+1)%2].clear();
    }

    return ret;
}