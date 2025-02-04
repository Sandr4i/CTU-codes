#include <omp.h>
#include <unordered_set>
#include <atomic>
#include <limits>
#include <queue>
#include "iddfs.h"

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi (respektive nej-
// levnejsi) cesty v grafu. V teto metode mate ze ukol naimplementovat pametove
// efektivni algoritmus pro prohledavani velkeho stavoveho prostoru. Pocitejte
// s tim, ze Vami navrzeny algoritmus muze bezet na stroji s omezenym mnozstvim
// pameti (radove nizke stovky megabytu). Vhodnym pristupem tak muze byt napr.
// iterative-deepening depth-first search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi/nejlevnejsi cesty.

/*void fill_data(state_ptr state, std::vector<state_ptr>& data){
    auto max_threads = omp_get_max_threads();
    auto next_states = state->next_states();

    for (const auto & next_state : next_states){
        data.push_back(next_state);
    }

    while(data.size() < static_cast<size_t>(max_threads)){
        auto s = data.back();

        if(!s->is_goal()) data.pop_back();
        auto nexts = s->next_states();

        for(const auto& next_state : nexts){
            data.push_back(next_state);
        }
    }
}

state_ptr find_depth(state_ptr state, unsigned int depth){
    if(state->is_goal()) return state;
    else if(depth == 0) return nullptr;

    auto next_states = state->next_states();
    auto prevId = state->get_predecessor()->get_identifier();
    for(const auto & next : next_states){
        if(next->get_identifier() == prevId) continue;

        auto ret = find_depth(next, depth-1);
        if(ret) return ret;
    }
    return nullptr;
}*/

#define MAX_THREADS 8

state_ptr iddfs(state_ptr root) {
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
    while (static_cast<size_t>(threads_num * 30000) > queue[level].size()) {
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
