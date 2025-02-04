#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define _PHASE_COUNT 6

_Bool invalid_command = true;

enum place {
  NUZKY, VRTACKA, OHYBACKA, SVARECKA, LAKOVNA, SROUBOVAK, FREZA,
  _PLACE_COUNT
};

const char *place_str[_PLACE_COUNT] = {
        [NUZKY] = "nuzky",
        [VRTACKA] = "vrtacka",
        [OHYBACKA] = "ohybacka",
        [SVARECKA] = "svarecka",
        [LAKOVNA] = "lakovna",
        [SROUBOVAK] = "sroubovak",
        [FREZA] = "freza",
};

enum product {
  A, B, C,
  _PRODUCT_COUNT
};

const char *product_str[_PRODUCT_COUNT] = {
        [A] = "A",
        [B] = "B",
        [C] = "C",
};

typedef struct worker{
    int place;
    char* name;
    _Bool working;
    _Bool work_ended;
    _Bool gone_home;
} worker_s;

_Bool working = false;

pthread_mutex_t work;

pthread_cond_t condition;
pthread_cond_t conditions[_PLACE_COUNT];

int ready_places[_PLACE_COUNT];
int parts[_PRODUCT_COUNT][_PHASE_COUNT];
int busy_places[_PLACE_COUNT];

const int make_a[_PHASE_COUNT] = {
    NUZKY,
    VRTACKA,
    OHYBACKA,
    SVARECKA,
    VRTACKA,
    LAKOVNA
};

const int make_b[_PHASE_COUNT] = {
    VRTACKA,
    NUZKY,
    FREZA,
    VRTACKA,
    LAKOVNA,
    SROUBOVAK
};

const int make_c[_PHASE_COUNT] = {
    FREZA,
    VRTACKA,
    SROUBOVAK,
    VRTACKA,
    FREZA,
    LAKOVNA
};

const int work_timer[_PLACE_COUNT] = {
    [NUZKY] = 100000,
    [VRTACKA] = 200000,
    [OHYBACKA] = 150000,
    [SVARECKA] = 300000,
    [LAKOVNA] = 400000,
    [SROUBOVAK] = 250000,
    [FREZA] = 500000
};

int find_string_in_array(const char **array, int length, char *what) {
   for (int i = 0; i < length; i++)
      if (strcmp(array[i], what) == 0)
         return i;
   return -1;
}

void start_work(worker_s *worker){
    usleep(work_timer[worker->place]);
}

void *production(void *workers){
    worker_s *worker = (worker_s*)workers;
    while(1){
        int type = -1;
        int phase = -1;
        pthread_mutex_lock(&work);
        if (worker->work_ended){
            pthread_mutex_unlock(&work);
            worker->work_ended = true;
            printf("%s goes home\n", worker->name);
            return NULL;
        }
        if (worker->place >= 0 && worker->place < _PLACE_COUNT && ready_places[worker->place] > 0){
            int i = _PHASE_COUNT-1;
            while( i >= 0 ){
                if (worker->place == make_a[i] && (parts[A][i] >= 1)){
                    type = A;
                    phase = i;
                }
                else if (worker->place == make_b[i] && (parts[B][i] >= 1)){
                    type = B;
                    phase = i;
                }
                else if (worker->place == make_c[i] && (parts[C][i] >= 1)){
                    type = C;
                    phase = i;
                }
                i--;
            }
            if (!working){
                i = _PHASE_COUNT - 1;
                while(i >= 0){
                    if ((parts[A][i] == 1)&& (i+1 != 6) && (!working)){
                        if ((parts[A][i+1] == 0) && (busy_places[make_a[i+1]] < 1)) pthread_cond_signal(&condition);
                    }
                    if ((parts[B][i] == 1) && (i+1 != 6) && (!working)){
                        if ((parts[B][i+1] == 0) && (busy_places[make_b[i+1]] < 1)) pthread_cond_signal(&condition);
                    }
                    if ((parts[C][i] == 1) && (i+1 != 6) && (!working)){
                        if ((parts[C][i+1] == 0) && (busy_places[make_c[i+1]] < 1)) pthread_cond_signal(&condition);
                    }
                    i--;
                }
            }
            if ((phase != -1) && (type != -1)){
                pthread_cond_signal(&condition);
                
                printf("%s %s %d %s\n", worker->name, place_str[worker->place], phase+1, product_str[type]);
                
                if (ready_places[worker->place] > 0) ready_places[worker->place] -= 1;
                parts[type][phase] -= 1;
                worker->working = true;
                
                pthread_mutex_unlock(&work);
                
                start_work(worker);
                pthread_mutex_lock(&work);
                worker->working = false;
                ready_places[worker->place] += 1;
                if (phase == _PHASE_COUNT-1) printf("done %s\n", product_str[type]);
                else{
                    parts[type][phase+1] += 1;
                    switch (type) {
                        case(A):
                            pthread_cond_broadcast(&conditions[make_a[phase+1]]);
                            break;
                        case(B):
                            pthread_cond_broadcast(&conditions[make_b[phase+1]]);
                            break;
                        case(C):
                            pthread_cond_broadcast(&conditions[make_c[phase+1]]);
                            break;
                        default:
                            break;
                    }
                }
                pthread_cond_signal(&condition);
            }
            else {
                pthread_cond_wait(&conditions[worker->place], &work);
            }
        }
        else {
            pthread_cond_wait(&conditions[worker->place], &work);
        }
        pthread_mutex_unlock(&work);
    }
    worker->work_ended = true;
    printf("%s goes home\n", worker->name);
    return NULL;
}

void *safeAlloc(unsigned long size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

int main(int argc, char **argv) {
    /* init your internal structures, mutexes and conddition variables */
    int workers_count = 0;
    worker_s **workers;
    workers = (worker_s**)safeAlloc(sizeof(worker_s) * 30);
    for (int i = 0; i < 30; i++){
        workers[i] = (worker_s*)safeAlloc(sizeof(worker_s));
    }
    pthread_t *work_thds;
    work_thds = (pthread_t*)safeAlloc(sizeof(pthread_t)*30);
    
    pthread_mutex_init(&work, NULL);
    pthread_cond_init(&condition, NULL);

    for (int i = 0; i < _PLACE_COUNT; i++){
        pthread_cond_init(&conditions[i], NULL);
        ready_places[i] = 0;
        busy_places[i] = 0;
    }

    for (int i = 0; i < _PRODUCT_COUNT; i++){
        for (int j = 0; j < _PHASE_COUNT; j++){
            parts[i][j] = 0;
        }
    }

    char *line, *cmd, *arg1, *arg2, ch;
    while (1) {
        int s = scanf(" %m[^\n]", &line), s2;
        if (s == 1) {
            if (strncmp(line, "start", 5) == 0) {
                invalid_command = false;
                s2 = sscanf(line, "%m[a-zA-Z] %m[a-zA-Z] %m[a-zA-Z]%c", &cmd, &arg1, &arg2, &ch);
                if ((s2 == 3) && (strcmp(cmd, "start") == 0)) {
                    /* start new thread for new worker */
                    /* copy worker name from arg1, the arg1 will be removed at the end of scanf
                     * cycle */
                    /* workers should have dynamic objects, you don't know total number of workers
                     */
                    int ret = find_string_in_array(place_str, _PLACE_COUNT, arg2);
                    if (ret == -1){
                        free(line);
                        free(cmd);
                        free(arg1);
                        free(arg2);
                        continue;
                    }
                    workers[workers_count]->place = ret;
                    workers[workers_count]->name = strdup(arg1);
                    workers[workers_count]->working = false;
                    workers[workers_count]->work_ended = false;
                    busy_places[find_string_in_array(place_str, _PLACE_COUNT, arg2)] += 1;
                
                    pthread_create(&work_thds[workers_count], NULL, production, workers[workers_count]);
                    workers_count++;
                }
                if (s2 >= 3) {
                    free(arg2);
                }
                
            }
            else {
                s2 = sscanf(line, "%m[a-zA-Z] %m[a-zA-Z]%c", &cmd, &arg1, &ch);
                if ((s2 == 2) && (strcmp(cmd, "make") == 0)) {
                    invalid_command = false;
                    /* int product = find_string_in_array(product_str, _PRODUCT_COUNT, arg1);
                     *
                     * if (product>=0) {.....
                     *   add the part to factory cycle
                     *   you need to wakeup worker to start working if possible
                     */
                    int product = find_string_in_array(product_str, _PRODUCT_COUNT, arg1);
                    if (product >= 0){
                        pthread_mutex_lock(&work);
                        parts[product][0]++;
                        switch(product){
                            case(A):
                                pthread_cond_broadcast(&conditions[make_a[0]]);
                                break;
                            case(B):
                                pthread_cond_broadcast(&conditions[make_b[0]]);
                                break;
                            case(C):
                                pthread_cond_broadcast(&conditions[make_c[0]]);
                                break;
                            default:
                                break;
                                
                        }
                        pthread_mutex_unlock(&work);
                    }
                }
                else if ((s2 == 2) && (strcmp(cmd, "end") == 0)) {
                    invalid_command = false;
                    /* tell the worker to finish
                     * the worker has to finish work first
                     * you should not wait here for the worker to finish */
                    /* if the worker is waiting for work
                       you need to wakeup the worker */
                    for (int i = 0; i < workers_count; i++){
                        if (strcmp(workers[i]->name, strdup(arg1)) == 0){
                            pthread_mutex_lock(&work);
                            workers[i]->work_ended = true;
                            busy_places[workers[i]->place] -= 1;
                            workers[i]->gone_home = true;
                            pthread_cond_broadcast(&condition);
                            pthread_mutex_unlock(&work);
                        }
                    }
                } 
                else if ((s2 == 2) && (strcmp(cmd, "add") == 0)) {
                    invalid_command = false;
                    /* add new place */
                    /* if worker and part is ready, start working - wakeup worker */
                    pthread_mutex_lock(&work);
                    int ret = find_string_in_array(place_str, _PLACE_COUNT, arg1);
                    if (ret == -1){
                        free(line);
                        free(cmd);
                        free(arg1);
                        free(arg2);
                        continue;
                    }
                    ready_places[ret] += 1;
                    pthread_cond_signal(&condition);
                    pthread_mutex_unlock(&work);
                } 
                else if ((s2 == 2) && (strcmp(cmd, "remove") == 0)) {
                    invalid_command = false;
                    /* if you cannot remove empty place you cannot wait for finish work */
                    pthread_mutex_lock(&work);
                    int ret = find_string_in_array(place_str, _PLACE_COUNT, arg1);
                    if (ret == -1){
                        free(line);
                        free(cmd);
                        free(arg1);
                        free(arg2);
                        continue;
                    }
                    ready_places[ret] -= 1;
                    pthread_mutex_unlock(&work);
                }
            }
            if (invalid_command == true){
                free(arg2);
            }
            if (s2 > 0) {
                free(cmd);
            }
            if (s2 > 1) {
                free(arg1);
            }
            free(line);
        } 
        else if (s < 0) {
            break;
        }
    }
    /* check if every worker finished work and nobody can continue */
    while(1){
        _Bool someone_working = true;
        _Bool done = false;
        pthread_mutex_lock(&work);
        for (int i = 0; i < workers_count; i++){
            if (workers[i]->working == true){
                working = true;
                break;
            }
            else {
                // if (workers[i]->gone_home){
                //     printf("%s goes home\n", workers[i]->name);
                //     free(workers[i]->name);
                //     free(workers[i]);
                // }
                working = false;
            }
        }
        if (!working){
            for (int i = 0; i < _PRODUCT_COUNT - 1; i++){
                if((ready_places[i] > 0) && (busy_places[i] > 0)){
                    for (int j = 0; j < _PHASE_COUNT; j++){
                        if (parts[i][j] == 1){
                            done = true;
                            break;
                        }
                    }
                }
                if(done) break;
            }
        }
        else{
            done = true;
            someone_working = false;
            pthread_cond_wait(&condition, &work);
        }
        if(!done){
            pthread_mutex_unlock(&work);
            break;
        }
        else if (someone_working){
            for(int i = 0; i < _PLACE_COUNT; i++){
                pthread_cond_broadcast(&conditions[i]);
            }
            pthread_cond_wait(&condition, &work);
        }
        pthread_mutex_unlock(&work);
    }
    
    pthread_mutex_lock(&work);
    for (int i = 0; i < workers_count; ++i){
        workers[i]->work_ended = true;
    }

    for(int i = 0; i < _PLACE_COUNT; ++i){
        pthread_cond_broadcast(&conditions[i]);
    }
    pthread_mutex_unlock((&work));

    for (int i = 0; i < workers_count; ++i){
        pthread_join(work_thds[i], NULL);
    }

    for (int i = 0; i < workers_count; i++){
        free(workers[i]->name);
    }

    for (int i = 0; i < 30; ++i){
        free(workers[i]);
    }
    free(workers);
    free(work_thds);

    for (int i = 0; i < _PLACE_COUNT; i++){
        pthread_cond_destroy(&conditions[i]);
    }
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&work);
    return 0;
}
