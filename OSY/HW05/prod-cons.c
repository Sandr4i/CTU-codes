#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Default number of consumer threads
#define DEFAULT_CONSUMERS 1

int consumers = DEFAULT_CONSUMERS;

pthread_mutex_t mutex_out, mutex_in;
sem_t consumer_sem;

typedef struct line{
    char *text;
    int number;
    struct line *next;
} line_s;

// First pair of number and text in data queue
line_s *start;

//volatile _Bool consumer_state = true, producer_state = true;

/* Function to write data from stdin to queue
    TEXT - is a text
    x - is number
*/
void add(char *text, unsigned int x);

// Function to read data from data queue
line_s *pop();

// Function (thread) that reads stdin
void *producer();

/* Function to process to control consumer threads activity
    ARG - id of consumer thread
*/
void *consumer(void *arg);

// Main process (reading data, printig data, controlling and synchronizing threaeds)
int main_pocess();

int main(int argc, char **argv){
    int code = 0;
    if (argc == 2) consumers = atoi(argv[1]);
    if (!(consumers >= 1 && consumers <= sysconf(_SC_NPROCESSORS_ONLN))){
        fprintf(stderr, ("Bad argument!\n"));
        exit(1);
    }

    pthread_mutex_init(&mutex_out, NULL);
    pthread_mutex_init(&mutex_in, NULL);
    sem_init(&consumer_sem, 1, 0);

    code = main_pocess();
    pthread_mutex_destroy(&mutex_in);
    pthread_mutex_destroy(&mutex_out);
    sem_destroy(&consumer_sem);
    return code;
}

int main_pocess(){
    pthread_t *consumers_thd = (pthread_t *) malloc(sizeof(pthread_t)*consumers);
    pthread_t producer_thd;

    pthread_create(&producer_thd, NULL, producer, NULL);
    
    int num[consumers];
    for (int i = 0; i < consumers; i++){
        num[i] = i+1;
        pthread_create(&consumers_thd[i], NULL, consumer, (void *) &num[i]);
    }

    int ret = 0;
    if (pthread_join(producer_thd, (void *)(&ret)) != 0){
        perror("Failed to join producers thread.\n");
        exit(1);
    }
    for (int i = 0; i < consumers; i++){
        pthread_join(consumers_thd[i], NULL);
    }

    while (start != NULL){
        line_s *temp = start;
        start=start->next;
        if (temp->text != NULL) free(temp->text);
        free(temp);
    }
    free(consumers_thd);
    return ret;
}


void *producer(){
    int res;
    int x;
    char *text;
    while((res = scanf("%d %ms", &x, &text)) == 2){
        if (x < 0) {
                fprintf(stderr, "Bad input!\n");
                free(text);
                sem_post(&consumer_sem);
                return (void *)1;
        }
        pthread_mutex_lock(&mutex_in);
        add(text, x);
        pthread_mutex_unlock(&mutex_in);
        sem_post(&consumer_sem);
    }
    if (res != EOF){
        sem_post(&consumer_sem);
        return (void *)1;
    }
    for (int i = 0; i < consumers; i++){
        sem_post(&consumer_sem);
    }
    return NULL;;
}

void *consumer(void *arg){
    int *idx = (int *)arg;
    while(1) { 
        sem_wait(&consumer_sem);
        pthread_mutex_lock(&mutex_in);
        line_s *line = pop();
        pthread_mutex_unlock(&mutex_in);
        if (line != NULL){
            pthread_mutex_lock(&mutex_out);
            printf("Thread %d:", *idx);
            for (int i = 0; i < line->number; i++){
                printf(" %s", line->text);
            }
            printf("\n");
            fflush(stdout);
            pthread_mutex_unlock(&mutex_out);
            free(line->text);
            free(line);
            line = NULL;
        }
        else{
            sem_post(&consumer_sem);
            break;
        }
    }
    return NULL;
}

void add(char *text, unsigned int x){
    line_s *line = (line_s *)malloc(sizeof(line_s));
    line->text = text;
    line->number = x;
    line->next = NULL;
    
    if (start == NULL) start = line;
    else{
        line_s *point = start;
        while (point->next != NULL) {
            point = point->next;
        }
        point->next = line;
    }
}

line_s *pop(){
    if (start != NULL){
        line_s *ret = (line_s *)malloc(sizeof(line_s));
        ret->text = strdup(start->text);
        ret->number = start->number;
        ret->next = NULL;

        line_s *temp = start;
        if(start->next != NULL) start = start->next;
        else start = NULL;
        if(temp->next != NULL){
            free(temp->text);
            temp->text = NULL;
        }
        free(temp->text);
        free(temp);
        temp = NULL;
        return ret;
    }
    return NULL;
}