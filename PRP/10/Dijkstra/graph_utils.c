#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_utils.h"

#define START_SIZE 10

graph_st* allocate_graph() 
{
    graph_st *graph = malloc(sizeof(graph_st));
    if (graph == NULL) 
    {
        fprintf(stderr, "Cannot allocate graph\n");
        return NULL;
    }
    graph->capacity = START_SIZE;
    graph->edges = NULL;
    graph->e_number = 0;
    return graph;
}

graph_st* realloc_g(graph_st *graph)
{
    if (graph == NULL)
    {
        fprintf(stderr, "Nothing to reallocate\n");
        exit(0);
    }
    int i;
    if (graph->capacity == 0) i = START_SIZE;
    else i = graph->capacity * 2;
    edge_st *e = malloc(sizeof(edge_st)*i);
    if (e == NULL)
    {
        fprintf(stderr, "Cannot allocate\n");
        exit(0);
    }
    graph->edges = e;
    graph->capacity = i;
    return graph;
}

void print_g(graph_st *graph)
{
    if (graph == NULL)
    {
        fprintf(stderr, "Nothing to print\n");
        exit(0);
    }
    edge_st *e = graph->edges;
    for (int i = 0; i < graph->e_number; i++)
    {
        printf("%d %d %d\n", e->start, e->end, e->cost);
    }
}

void free_g(graph_st **graph) 
{
    if (graph == NULL && *graph == NULL)
    {
        fprintf(stderr, "Nothing to free\n");
        exit(0);
    }
    free(*graph);
    *graph = NULL;
}

int load_g(const char *fname, graph_st *g)
{
    int c = 0;
    int exit = 0;
    FILE *f = fopen(fname, "r");
    while (f && !exit)
    {
        if (g->e_number == g->capacity) realloc_g(g);
    
        edge_st *e = g->edges + g->e_number;
        while (g->e_number < g->capacity)
        {
            int r = fscanf(f, "%d %d %d\n", &(e->start), &(e->end), &(e->cost));
            if (r == 3)
            {
                g->e_number += 1;
                c += 1;
                e += 1;
            }
            else
            {
                exit = 1;
                break;
            }
        }
    }
    if (f)
    {
        fclose(f);
    }
    return c;
}


