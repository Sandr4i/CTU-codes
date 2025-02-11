#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define START_INIT 10

graph_t* allocate_graph() 
{
    graph_t *g = malloc(sizeof(graph_t));
    if (!g) 
    {
        fprintf(stderr, "Cannot allocate graph\n");
        return NULL;
    }
    g->size = 0;
    g->capacity = START_INIT;
    edge_t *e = malloc(sizeof(edge_t) * g->capacity);
    if (!e) 
    {
        free(g);
        fprintf(stderr, "Cannot allocate edges of graph\n");
        return NULL;
    }
    g->edges = e;
    return g;
}

void free_graph(graph_t **graph) 
{
    if ((*graph)->capacity != 0) free((*graph)->edges);
    (*graph)->capacity = 0;
    (*graph)->size = 0;
    free(*graph);
    *graph = NULL;
}

void realloc_capacity(graph_t *graph)
{
    graph->capacity *= 2;
    edge_t *temp = realloc(graph->edges, sizeof(edge_t) * graph->capacity);
    if (!temp) 
    {
        fprintf(stderr, "Cannot reallocate\n");
        free_graph(&graph);
        return;
    }
    graph->edges = temp;
}

int scan(FILE *f)
{
    char num[10];
    char digit;
    digit = fgetc(f);
    int i = -1;
    while (digit != ' ')
    {
        i++;
        num[i] = digit;
        digit = fgetc(f);
        if (digit == EOF) return -1;
        if (digit == 10 || digit == -1) break;
    }
    num[i + 1] = '\0';
    char *s = num;
    int ret = 0;
    while (*s)
    {
        ret = ret*10 + (*s++ - '0');
    }
    return ret;
}

void load_txt(const char *fname, graph_t *graph) 
{
    FILE *f = fopen(fname, "rt");
    if (!f) 
    {
        free_graph(&graph);
        fprintf(stderr, "Cant open txt file\n");
        exit(1);
    }
    int ret=0;
    while (!feof(f) && !ret)
    {
        if (graph->size == graph->capacity) realloc_capacity(graph);
        edge_t *e = graph->edges + graph->size;
        int from, to, cost;
        while (!feof(f) && (graph->size < graph->capacity))
        {
            from = scan(f);
            to = scan(f);
            cost= scan(f);
            if ( from < 0 || to < 0 || cost < 0)
            {
                ret = 1;
                break;
            }
            e->from = from;
            e->to = to;
            e->cost = cost;
            graph->size++;
            e++;
        }
    }
    fclose(f);
}   

void load_bin(const char *fname, graph_t *graph)
{
    FILE *b = fopen (fname, "rb");
    if (!b)
    {
        free_graph(&graph);
        fprintf(stderr, "Cant open bin file\n");
        exit(1);
    }
    if (graph->size == graph->capacity) realloc_capacity(graph);
    while (!feof(b))
    {
        edge_t *e = graph->edges + graph->size;
        if (graph->size < graph->capacity)
        {
            if (fread(e, sizeof(edge_t), 1, b)!=1) break;
            graph->size++;
            e++;
        }
        else realloc_capacity(graph);
    }
    fclose(b);
}

void put(FILE *f, int i)
{
    if (i>9)
    {
        int c = i/10;
        i = i - (10*c);
        put(f, c);
    }
    fputc('0'+i, f);
}

void save_txt(const graph_t * const graph, const char *fname)
{
    FILE *f = fopen(fname, "wt");
    edge_t *e = graph->edges;
    for (int i = 0; i<graph->size; i++, e++)
    {
        put(f, e->from);
        fputc(' ', f);
        put(f, e->to);
        fputc(' ', f);
        put(f, e->cost);
        fputc('\n', f);
    }
    fclose(f);
}

void save_bin(const graph_t * const graph, const char *fname)
{
    FILE *b = fopen(fname, "wb");
    edge_t *tmp = graph->edges;
    for ( int i = 0; i< graph->size; i++, tmp++)
    {
        fwrite(&tmp->from, sizeof(tmp->from), 1, b);
        fwrite(&tmp->to, sizeof(tmp->to), 1, b);
        fwrite(&tmp->cost, sizeof(tmp->cost), 1, b);

    }
    fclose(b);
}
