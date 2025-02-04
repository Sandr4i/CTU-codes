#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define WORKERS_CAPACITY 15
#define _PHASE_COUNT 6

enum place{
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

const int work_timer[_PLACE_COUNT] = {
    [NUZKY] = 1e8,
    [VRTACKA] = 2e8,
    [OHYBACKA] = 1.5e8,
    [SVARECKA] = 3e8,
    [LAKOVNA] = 4e8,
    [SROUBOVAK] = 2.5e8,
    [FREZA] = 5e8
};

typedef struct worker{
    int place;
    pthread_t id;
    char* name;
    _Bool work_ended;
} worker_s;

typedef struct place_s{
    int id;
    int count;
    int worker_count;
    int free_workers;
    int available;
} place_s;

typedef struct phase{
    int place;
    int working;
    int halfproducts;
} phase_s;

typedef struct factory{
    int workers_count;
    int capacity;
    int places_count;
    place_s *places;
    worker_s *workers;
    phase_s **phases;
    int is_active;
} factory_s;

typedef struct worker_info_s{
    factory_s *factory;
    worker_s *worker;
} worker_info_s;

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

pthread_mutex_t factory_mutex;
pthread_cond_t condition;

int find_string_in_array(const char **array, int length, char *what) {
   for (int i = 0; i < length; i++)
      if (strcmp(array[i], what) == 0)
         return i;
   return -1;
}

void *safeAlloc(unsigned long size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(1);
    }
    return mem;
}

factory_s *open_facroty(){
    factory_s *factory = (factory_s*)safeAlloc(sizeof(factory_s));
    factory->is_active = 1;
    factory->places_count = _PLACE_COUNT;
    factory->places = (place_s*)safeAlloc(sizeof(place_s) * _PLACE_COUNT);
    for (int i = 0; i < _PLACE_COUNT; i++){
        factory->places[i].id = i;
        factory->places[i].available = 0;
        factory->places[i].free_workers = 0;
        factory->places[i].worker_count = 0;
        factory->places[i].count = 0;
    }
    factory->capacity = WORKERS_CAPACITY;
    factory->workers_count = 0;
    factory->workers = (worker_s*)safeAlloc(sizeof(worker_s) * WORKERS_CAPACITY);
    factory->phases = (phase_s**)safeAlloc(sizeof(phase_s*) * _PRODUCT_COUNT);
    for (int i = 0; i < _PRODUCT_COUNT; i++){
        factory->phases[i] = (phase_s*)safeAlloc(sizeof(phase_s) * _PHASE_COUNT);
        for (int j = 0; j < _PHASE_COUNT; j++){
            if (i == 0) factory->phases[i][j].place = make_a[j];
            else if (i == 1) factory->phases[i][j].place = make_b[j];
            else if (i == 2) factory->phases[i][j].place = make_c[j];
            factory->phases[i][j].halfproducts = 0;
            factory->phases[i][j].working = 0;
        }
    }
    return factory;
}

void process_phase(int place, int product, int step, char *name){
    struct timespec sleep = { .tv_sec = 0, .tv_nsec = work_timer[place]};
    struct timespec remain;
    printf("%s %s %d %s\n", name, place_str[place], step, product_str[product]);
    nanosleep(&sleep, &remain);
}

int get_job(worker_s *worker, factory_s *factory){
    for (int i = 5; i >= 0; i--){
        for (int j = 0; j < _PRODUCT_COUNT; j++){
            pthread_mutex_lock(&factory_mutex);
            if (worker->place == factory->phases[j][i].place && factory->phases[j][i].halfproducts > 0 && factory->places[worker->place].available > 0){
                factory->phases[j][i].halfproducts--;
                factory->phases[j][i].working++;
                factory->places[worker->place].available--;
                factory->places[worker->place].free_workers--;
                pthread_mutex_unlock(&factory_mutex);
                process_phase(worker->place, j, i+1, worker->name);
                pthread_mutex_lock(&factory_mutex);
                factory->places[worker->place].available++;
                factory->places[worker->place].free_workers++;
                factory->phases[j][i].working--;
                if(i == 5) printf("done %s\n", product_str[j]);
                else factory->phases[j][i+1].halfproducts++;
                pthread_mutex_unlock(&factory_mutex);
                pthread_cond_broadcast(&condition);
                return 1;
            }
            pthread_mutex_unlock(&factory_mutex);
        }
    }
    return 0;
}

int work_in_progress(int product, factory_s *factory){
    for(int i = 0; i < _PHASE_COUNT; i++){
        if (factory->phases[product][i].working) return 1;
    }
    return 0;
}

int can_work(worker_s *worker, int product, factory_s *factory){
    if(worker->work_ended) return 0;
    for(int i = 0; i < _PHASE_COUNT; i++){
        if(worker->place == factory->phases[product][i].place && factory->phases[product][i].halfproducts > 0 && factory->places[worker->place].count > 0) return 1;
    }
    return 0;
}

void *work(void *smth){
    worker_info_s *data = (worker_info_s*)smth;
    worker_s *worker = data->worker;
    factory_s *factory = data->factory;
    int stop;

    while(1){
        if(worker->work_ended){
            pthread_mutex_lock(&factory_mutex);
            factory->places[worker->place].worker_count--;
            if(factory->places[worker->place].free_workers > factory->places[worker->place].worker_count){
                factory->places[worker->place].free_workers--;
            }
            pthread_mutex_unlock(&factory_mutex);
            break;
        }
        if(get_job(worker, factory)) continue;
        pthread_mutex_lock(&factory_mutex);
        if(factory->is_active == 0){
            stop = 1;
            switch(worker->place){
                case(NUZKY):
                    stop = ((work_in_progress(A, factory) || work_in_progress(B, factory)) ? 0 : 1);
                    break;
                case(VRTACKA):
                    stop = ((work_in_progress(A, factory) || work_in_progress(B, factory) || work_in_progress(C, factory)) ? 0 : 1);
                    break;
                case(OHYBACKA):
                    stop = work_in_progress(A, factory) ? 0 : 1;
                    break;
                case(SVARECKA):
                    stop = work_in_progress(A, factory) ? 0 : 1;
                    break;
                case(LAKOVNA):
                    stop = ((work_in_progress(A, factory) || work_in_progress(B, factory) || work_in_progress(C, factory)) ? 0 : 1);
                    break;
                case(SROUBOVAK):
                    stop = ((work_in_progress(C, factory) || work_in_progress(B, factory)) ? 0 : 1);
                    break;
                case(FREZA):
                    stop = ((work_in_progress(C, factory) || work_in_progress(B, factory)) ? 0 : 1);
                    break;
                default:
                    fprintf(stderr, "ERROR: Wrong working place!\n");
                    break;
            }
            if (stop){
                for (int i = 0; i < factory->workers_count; i++){
                    worker_s *temp = &(factory->workers[i]);
                    if(worker->place == NUZKY && (can_work(temp, A, factory) || can_work(temp, B, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == VRTACKA && (can_work(temp, A, factory) || can_work(temp, B, factory) || can_work(temp, C, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == OHYBACKA && (can_work(temp, A, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == SVARECKA && (can_work(temp, A, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == LAKOVNA && (can_work(temp, A, factory) || can_work(temp, B, factory) || can_work(temp, C, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == SROUBOVAK && (can_work(temp, B, factory) || can_work(temp, C, factory))){
                        stop = 0;
                        break;
                    }
                    else if(worker->place == FREZA && (can_work(temp, B, factory) || can_work(temp, C, factory))){
                        stop = 0;
                        break;
                    }
                }
                if(stop){
                    worker->work_ended = 1;
                    pthread_mutex_unlock(&factory_mutex);
                    pthread_cond_broadcast(&condition);
                    continue;
                }
            }
        }
        pthread_cond_wait(&condition, &factory_mutex);
        pthread_mutex_unlock(&factory_mutex);
    }
    printf("%s goes home\n", worker->name);
    free(data);
    worker = NULL;
    pthread_exit(NULL);
}

void add_worker(char *name, int place, factory_s *factory){
    pthread_mutex_lock(&factory_mutex);
    
    factory->workers[factory->workers_count].name = name;
    factory->workers[factory->workers_count].place = place;
    factory->workers[factory->workers_count].work_ended = 0;
    factory->places[place].free_workers++;
    factory->places[place].worker_count++;
    int i = factory->workers_count;
    factory->workers_count++;
    pthread_mutex_unlock(&factory_mutex);
    
    worker_info_s *data = (worker_info_s*)safeAlloc(sizeof(worker_info_s));
    data->worker = &(factory->workers[i]);
    data->factory = factory;

    pthread_create(&(factory->workers[i].id), NULL, &work, (void*)data);
}

void open_place(int place, factory_s *factory){
    pthread_mutex_lock(&factory_mutex);
    factory->places[place].available++;
    factory->places[place].count++;
    pthread_mutex_unlock(&factory_mutex);
    pthread_cond_broadcast(&condition);
}

void make(int product, factory_s *factory){
    pthread_mutex_lock(&factory_mutex);
    factory->phases[product][0].halfproducts++;
    pthread_mutex_unlock(&factory_mutex);
    pthread_cond_broadcast(&condition);
}

int stop_worker(char *name, factory_s *factory){
    pthread_mutex_lock(&factory_mutex);
    for (int i = 0; i < factory->workers_count; i++){
        if (strcmp(factory->workers[i].name, name) == 0){
            factory->workers[i].work_ended = 1;
            pthread_mutex_unlock(&factory_mutex);
            pthread_cond_broadcast(&condition);
            return 1;
        }
    }
    pthread_mutex_unlock(&factory_mutex);
    return 0;
}

void close_place(int place, factory_s *factory){
    pthread_mutex_lock(&factory_mutex);
    if (factory->places[place].count > 0) factory->places[place].count--;
    factory->places[place].available--;
    pthread_mutex_unlock(&factory_mutex);
    pthread_cond_broadcast(&condition);
}

void close_facroty(factory_s *factory){
    free(factory->places);
    for (int  i = 0; i < factory->workers_count; i++){
        free(factory->workers[i].name);
    }
    free(factory->workers);

    for (int i = 0; i < _PRODUCT_COUNT; i++){
        free(factory->phases[i]);
    }
    free(factory->phases);

    free(factory);
    factory = NULL;
}

int main(){
    pthread_mutex_init(&factory_mutex, NULL);
    pthread_cond_init(&condition, NULL);
    factory_s *factory = open_facroty();
    char *line, *cmd, *arg1, *arg2, *arg3, *saveptr;
    while (1) {
        int ret = scanf(" %m[^\n]", &line);
        if (ret == EOF) break;
        if (ret == 0) continue;

        cmd = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);

        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3){
            int place = find_string_in_array(place_str, _PLACE_COUNT, arg2);
            if (place >= 0) add_worker(strdup(arg1), place, factory);
            else fprintf(stderr, "ERROR: Wrong working place!\n");
        }
        else if (strcmp(cmd, "make") == 0 && arg1 && !arg2){
            int product = find_string_in_array(product_str, _PRODUCT_COUNT, arg1);
            if (product >= 0) make(product, factory);
            else fprintf(stderr, "ERROR: Wrong product!\n");
        }
        else if (strcmp(cmd, "end") == 0 && arg1 && !arg2){
            if (!stop_worker(arg1, factory)) fprintf(stderr, "ERROR: Wrong worker!\n");
        }
        else if (strcmp(cmd, "add") == 0 && arg1 && !arg2){
            int place = find_string_in_array(place_str, _PLACE_COUNT, arg1);
            if (place >= 0) open_place(place, factory);
            else fprintf(stderr, "ERROR: Wrong place!\n");
        }
        else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2){
            int place = find_string_in_array(place_str, _PLACE_COUNT, arg1);
            if (place >= 0) close_place(place, factory);
            else fprintf(stderr, "ERROR: Wrong place!\n");
        }
        else fprintf(stderr, "ERROR: Wrong command!\n");
        free(line);
    }

    pthread_mutex_lock(&factory_mutex);
    factory->is_active = 0;
    pthread_mutex_unlock(&factory_mutex);

    for (int i = 0; i < factory->workers_count; i++){
        if (pthread_join(factory->workers[i].id, NULL) != 0) perror("ERROR: Failed while joining thread!\n");
    }
    close_facroty(factory);
    pthread_mutex_destroy(&factory_mutex);
    pthread_cond_destroy(&condition);
    return 0;
}
