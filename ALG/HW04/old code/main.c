#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int distance;
    uint8_t visited;
} cell_s;

typedef struct {
    int height;
    int width;
    int colors;
    int8_t **field;
    cell_s ***graph;
} game_s;

typedef struct vertex_s {
    int row, col;
    int value;
    int distance;
    struct vertex_s *next;
} vertex_s;

typedef struct  {
    int size;
    vertex_s *head;
    vertex_s *tail;
} vertex_list_s;

typedef struct  {
    int row, col;
    _Bool empty;
    int distance;
    int value;
} queue_s;

void *safeAllocation(int size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

game_s *createGame(int height, int width, int colors){
    game_s *g = (game_s*)safeAllocation(sizeof(game_s));
    g->height = height;
    g->width = width;
    g->colors = colors;
    g->field = (int8_t**)safeAllocation(height*sizeof(int8_t*));
    g->graph = (cell_s ***)safeAllocation((colors + 1)*sizeof(cell_s**));
    for (int i = 0; i <= colors; i++){
        g->graph[i] = (cell_s**)safeAllocation(height*sizeof(cell_s*));
    }
    for (int i = 0; i < height; i++){
        g->field[i] = (int8_t*)safeAllocation(width*sizeof(int8_t));
        for (int k = 0; k <= colors; k++){
            g->graph[k][i] = (cell_s*)safeAllocation(width*sizeof(cell_s));
        }
        for (int j = 0; j < width; j++){
            g->field[i][j] = 0;
            for (int k = 0; k <= colors; k++){
                g->graph[k][i][j].visited = 0;
                g->graph[k][i][j].distance = -1;
            }
        }
    }
    return g;
}

vertex_list_s *createList(){
    vertex_list_s *l = (vertex_list_s*)safeAllocation(sizeof(vertex_list_s));
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

queue_s *createQueue(){
    queue_s *q = (queue_s*)safeAllocation(sizeof(queue_s));
    q->row = 0;
    q->col = 0;
    q->empty = 1;
    q->distance = 0;
    q->value = 0;
    return q;
}

void add(vertex_list_s *list, int row, int col, int distance, int value){
    vertex_s *v = (vertex_s*)safeAllocation(sizeof(vertex_s));
    v->row = row;
    v->col = col;
    v->distance = distance;
    v->value = value;
    v->next = NULL;

    if(list->tail == NULL) list->head = v;
    else list->tail->next = v;
    list->tail = v;
    list->size++;
}
void pop(vertex_list_s *list, queue_s *queue){
    if (list->head == NULL) {
        queue->empty = 1;
        return;
    }
    queue->row = list->head->row;
    queue->col = list->head->col;
    queue->value = list->head->value;
    queue->distance = list->head->distance;
    queue->empty = 0;

    vertex_s *temp = list->head;
    list->head = list->head->next;
    if (list->head == NULL) list->tail = NULL;
    free(temp);
    list->size--;
}

int visit(game_s *area, int row, int col, int graph){
    if (row < 0 
        || row >= area->height 
        || col < 0 
        || col >= area->width
        || area->graph[graph][row][col].visited == 1
        || (area->field[row][col] > 0 && area->field[row][col] != graph)) {
            return 0;
        }
    return 1;
}

void findBestPath(game_s *area, vertex_list_s *list, int row, int col){
    queue_s *queue = createQueue();
    uint8_t cell;
    add(list, row, col, 0, 0);
    while (list->size > 0){
        pop(list, queue);
        if (!area->graph[queue->value][queue->row][queue->col].visited){
            area->graph[queue->value][queue->row][queue->col].visited = 1;
            area->graph[queue->value][queue->row][queue->col].distance = queue->distance;
            
            if(visit(area, queue->row - 1, queue->col, queue->value)){
                cell = abs(area->field[queue->row - 1][queue->col]);
                if (area->field[queue->row - 1][queue->col] < 0
                    && cell != queue->value) add(list, queue->row - 1, queue->col, queue->distance + 1, cell);
                else add(list, queue->row - 1,queue->col, queue->distance + 1, queue->value);
            }

            if (visit(area, queue->row, queue->col + 1, queue->value)) {
                cell = abs(area->field[queue->row][queue->col + 1]);
                if (area->field[queue->row][queue->col + 1] < 0
                    && cell != queue->value) add(list, queue->row, queue->col + 1, queue->distance+1, cell);
                else add(list, queue->row,queue->col + 1, queue->distance + 1, queue->value);
            }

            if (visit(area, queue->row + 1, queue->col, queue->value)) {
                cell = abs(area->field[queue->row + 1][queue->col]);
                if (area->field[queue->row + 1][queue->col] < 0
                    && cell != queue->value) add(list, queue->row + 1, queue->col, queue->distance+1, cell);
                else add(list, queue->row + 1 ,queue->col, queue->distance + 1, queue->value);
            }

            if (visit(area, queue->row, queue->col - 1, queue->value)) {
                cell = abs(area->field[queue->row][queue->col - 1]);
                if (area->field[queue->row][queue->col - 1] < 0
                    && cell != queue->value) add(list, queue->row, queue->col - 1, queue->distance+1, cell);
                else add(list, queue->row,queue->col - 1, queue->distance + 1, queue->value);
            }
        }
    }
    free(queue);
}

void freeGameArea(game_s *area){
    for (int i = 0; i < area->height; i ++){
        free(area->field[i]);
        for(int j = 0; j < area->colors + 1; j++){
            free(area->graph[j][i]);
        }
    }
    free(area->field);
    for (int j = 0; j < area->colors + 1; j++){
        free(area->graph[j]);
    }
    free(area->graph);
    free(area);
    area = NULL;
}

int main() {
    int M, N, C;
    if (scanf("%d %d %d", &M, &N, &C) != 3){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }
    //printf("FLAG1\n");
    game_s *area = createGame(M, N, C);
    //printf("FLAG2\n");
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            if (scanf("%hhd", &area->field[i][j]) != 1){
            fprintf(stderr, "ERROR: Couldn't read data!\n");
            exit(1);
            }
        }
    }
    vertex_list_s *list = createList();

    findBestPath(area, list, area->height-1,0);

    int distance, minimum = INT32_MAX;
    for (int i = 0; i <= area->colors; i ++){
        distance = area->graph[i][0][area->width-1].distance;
        if (distance != -1 && distance < minimum) minimum = distance;
    }
    freeGameArea(area);
    free(list);
    printf("%d", minimum);
    return 0;
}
