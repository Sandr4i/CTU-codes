#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

typedef struct node {
    int value;
    int depth;
    struct node *right;
    struct node *left;
} node_s;

void* safeAllocation(int size){
    void *mem = malloc(size);
    if (!mem){
        fprintf(stderr, "ERROR: allocation failed\n");
        exit(2);
    }
    return mem;
}

node_s* createNode(int value) {
    node_s *node = (node_s *)safeAllocation(sizeof(node_s));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

node_s* buildTree(int *nodes, int size) {
    node_s *root = createNode(nodes[0]);
    node_s *current = root;
    int t = -1;

    node_s **stack = (node_s **)safeAllocation(sizeof(node_s *) * size);
    for (int  i =1 ; i < size; i++){
        node_s *tmp = NULL;
        while(t != -1 && nodes[i] > stack[t]->value){
            current = stack[t];
            t--;
        }

        if (nodes[i] < current->value){
            tmp = createNode(nodes[i]);
            current->left = tmp; 
            t++;
            stack[t] = current;
            current = tmp;
        }
        else{
            current->right = createNode(nodes[i]);
            current = current->right;
        }
        t++;
        stack[t] = current;
    }
    return root;
}

int calculateDepth(node_s *node) {
    if(node == NULL) return 0;
    int leftDepth = calculateDepth(node->left);
    int rightDepth = calculateDepth(node->right);
    node->depth = (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
    return node->depth; 
}

int getDepth(node_s *node){
    return node->depth;
}

int buildRoad(node_s *node){
    if (node->left == NULL 
    && node->right == NULL) return 1;
    else if(node->right == NULL) return buildRoad(node->left) + 1;
    else if(node->left == NULL) return buildRoad(node->right) +1 ;
    else{
        int leftRoad, rightRoad;
        leftRoad = getDepth(node->right) + buildRoad(node->left) + 1;
        rightRoad = getDepth(node->left) + buildRoad(node->right) + 1;
        return (leftRoad > rightRoad ? leftRoad : rightRoad);
    } 
}
int main() {
    int N;
    node_s* root;
    if (scanf("%d", &N) != 1){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        return 1;
    }
    int *nodes = (int *)safeAllocation(sizeof(node_s*)*N);
    for(int i = 0; i < N; i++){
        if (scanf("%d", &nodes[i]) != 1){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        return 1;
        }
    }
    root = buildTree(nodes, N);
    calculateDepth(root);
    int ret = buildRoad(root);
    printf("%d\n", ret);
    return 0;
}


