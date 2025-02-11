#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct {
   __int32_t from;
   __int32_t to;
   __int32_t cost;
} edge_t;
 
typedef struct {
  edge_t *edges;
  __int32_t size;
  __int32_t capacity;
} graph_t;
 
/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);

/*Load graph from bin file*/
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname);

//===== Povinné zadání =====

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname);

#endif // __GRAPH_H__
