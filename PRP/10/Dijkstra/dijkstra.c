#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pq_heap.c"
#include "graph_utils.c"
#include "pq_heap.h"
#include "graph_utils.h"
#include "dijkstra.h"


/* I rewrited the code from lecture 11 file dijsktar.c
and added the functions that are needed for mandatory task - I added functions:
   
    _Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
    _Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3])

*/

typedef struct
{
    int start;  // first edge's index
    int cost;    // cost of path from start to current node
    int parent;  // index of parent node on path from start
    int counter;  // counter of number of edges
} node;

typedef struct
{
    int nodes_num;
    int start;
    node *nodes;
    graph_st *graph;
} dijkstra_st;

void* dijkstra_init(void)
{
    dijkstra_st *d = malloc(sizeof(dijkstra_st));
    if (d == NULL)
    {
        fprintf(stderr, "Cannot allocate\n");
        exit(0);
    }
    d->nodes = NULL;
    d->nodes_num = 0;
    d->start = -1;
    d->graph = allocate_graph();
    if (d->graph == NULL)
    {
        free(d);
        d = NULL;
    }
    return d;
}

_Bool dijkstra_load_graph(const char *fname, void *dijkstra)
{
    _Bool ret = false;
    dijkstra_st *d = (dijkstra_st*)dijkstra;
    if ( d && d->graph && load_g(fname, d->graph))
    {
        int m = -1;
        for (int i = 0; i < d->graph->e_number; i++)
        {
            const edge_st *const e = &(d->graph->edges[i]);
            m = m < e->start ? e->start : m;
            m = m < e->end ? e->end : m;
        }
        m += 1;
        d->nodes = malloc(sizeof(node)*m);
        if (d->nodes == NULL)
        {
            fprintf(stderr, "Cannot allocate\n");
            exit(0);
        }
        d->nodes_num = m;
        for (int i = 0; i < m; i++)
        {
            d->nodes[i].start = -1;
            d->nodes[i].counter = 0;
            d->nodes[i].parent = -1;
            d->nodes[i].cost = -1;
        }
        for (int i = 0; i < d->graph->e_number; i++)
        {
            int current = d->graph->edges[i].start;
            if (d->nodes[current].start == -1) d->nodes[current].start = i;
            d->nodes[current].counter += 1;
        }
        ret = true;
    }
    return ret;
}

_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
{
    _Bool ret = true;
    dijkstra_st *d = (dijkstra_st*)dijkstra;
    if (!d || !d->graph) ret = false;
    else
    {
        for (int i = 0; i < e; i++)
        {
            if (d->graph->e_number == d->graph->capacity) realloc_g(d->graph);
            edge_st *e = d->graph->edges + d->graph->e_number;
            e->start = edges[i][0];
            e->end = edges[i][1];
            e->cost = edges[i][2];
            d->graph->e_number++;
        }
        int j = -1;
        for (int i = 0; i < d->graph->e_number; i++)
        {
            edge_st *tmp = &(d->graph->edges[i]);
            if (j < tmp->start) j = tmp->start;
            if (j < tmp->end) j = tmp->end;
        }
        j++;
        d->nodes = malloc(sizeof(node)*j);  
        if (d->nodes==NULL)
        {
            fprintf(stderr, "Cannot allocate\n");
            exit(0);
        }
        d->nodes_num = j;
        for (int i = 0; i < j; i++)   // inicialize the nodes
        {
            d->nodes[i].counter = 0;
            d->nodes[i].cost = -1;
            d->nodes[i].parent = -1;
            d->nodes[i].start = -1;
        }
        for (int i = 0; i < d->graph->e_number; i++) // adding edges
        {
            int current = d->graph->edges[i].start;
            if (d->nodes[current].start == -1) d->nodes[current].start = i;
            d->nodes[current].counter++;
        }
    }
    return ret;
}

_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3])
{
    _Bool ret = true;
    dijkstra_st *d = (dijkstra_st*)dijkstra;
    
    if (!d) ret = false;
    else
    {
        for (int i = 0; i < d->nodes_num; i++)
        {
            solution[i][0] = i;
            solution[i][1] = d->nodes[i].cost;
            solution[i][2] = d->nodes[i].parent;
        }
    }
    return ret;
}

_Bool dijkstra_solve(void *dijkstra, int label)
{
    dijkstra_st *d = (dijkstra_st*)dijkstra;
    if (!d || label < 0 || label >= d->nodes_num) return false;
    d->start = label;
    void *pq = pq_alloc(d->nodes_num);
    d->nodes[label].cost = 0;
    pq_push(pq, label, 0);
    int current_label;
    while (!pq_is_empty(pq) && pq_pop(pq, &current_label))
    {
        node *c = &(d->nodes[current_label]);
        for (int i = 0; i < c->counter; i++)
        {
            edge_st *edge = &(d->graph->edges[c->start + i]);
            node *to = &(d->nodes[edge->end]);
            const int cost = c->cost + edge->cost;
            if (to->cost == -1)
            {
                to->cost = cost;
                to->parent = c->parent;
                pq_push(pq, edge->end, cost);
            }
            else if (cost < to->cost)
            {
                to->cost = cost;
                to->parent = current_label;
                pq_update(pq, edge->end, cost);
            }
        }
    }
    pq_free(pq);
    return true;
}

_Bool dijkstra_save_path(const void *dijkstra, const char *fname)
{
    _Bool ret = false;
    const dijkstra_st *const d = (dijkstra_st*)dijkstra;
    if (d)
    {
        FILE *f = fopen(fname, "w");
        if (f)
        {
            for (int i = 0; i < d->nodes_num; i++)
            {
                const node *const n = &(d->nodes[i]);
                fprintf(f, "%d %d %d\n", i, n->cost, n->parent);
            }
            ret = fclose(f) == 0;
        }
    }
    return ret;
}

void dijkstra_free(void *dijkstra)
{
    dijkstra_st *d = (dijkstra_st*)dijkstra;
    if (d)
    {
        if (d->graph)
        {
            free_g(&(d->graph));
        }
        if (d->nodes)
        {
            free(d->nodes);
        }
        free(d);
    }
}
