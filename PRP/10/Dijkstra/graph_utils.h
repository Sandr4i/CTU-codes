#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

typedef struct {
   int start;
   int end;
   int cost;
} edge_st;
 
typedef struct {
  edge_st *edges;
  int e_number;  //number of edges in graph
  int capacity;  
} graph_st;
 
/* Allocate a new graph and return a reference to it. */
graph_st* allocate_graph();

/*Function for enlarging graph*/
graph_st* realloc_g(graph_st *graph);

/*Function to print graph*/
void print_g(graph_st *graph);

/* Free all allocated memory and set reference to the graph to NULL. */
void free_g(graph_st **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_st *graph);

/*Load graph from bin file*/
void load_bin(const char *fname, graph_st *graph);

/* Save the graph to the text file. */
void save_txt(const graph_st * const graph, const char *fname);

/* Save the graph to the binary file. */
void save_bin(const graph_st *const graph, const char *fname);

int load_g(const char *fname, graph_st *g);

#endif 
