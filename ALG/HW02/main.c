#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


uint32_t bestTime = 0;
uint32_t bestBalance = 0;
uint32_t currentTime = 0;
uint8_t N, B;

typedef struct node {
    uint8_t idx;
    uint8_t valueIDX;
    uint16_t *values;
    uint8_t size;
    uint16_t time_right;
    uint16_t time_left;
    struct node *left;
    struct node *right;
    
} node_s;

typedef struct tree_s{
    node_s* root;
    node_s *nodes;
    uint8_t capacity;
} tree_s;



void* safeAllocation(uint32_t size){
    void *mem = malloc(size);
    if (!mem){
        fprintf(stderr, "ERROR: allocation failed\n");
        exit(2);
    }
    return mem;
}

tree_s* createTree(int size, int elements){
    tree_s* tree = (tree_s *)safeAllocation(sizeof(tree_s));
    tree->nodes = (node_s *)safeAllocation(sizeof(node_s)*size);
    for (int i = 0; i < size; i++){
        tree->nodes[i].idx=i;
        tree->nodes[i].left = NULL;
        tree->nodes[i].right = NULL;
        tree->nodes[i].time_left = 0;
        tree->nodes[i].time_right = 0;
        tree->nodes[i].values = (uint16_t*)safeAllocation(sizeof(uint16_t)*elements);
        tree->nodes[i].valueIDX = 0;
        tree->nodes[i].size = elements;
    }
    tree->root = &tree->nodes[0];
    tree->capacity = size;
    return tree;
}

void connect(tree_s* tree, uint8_t parent, uint8_t child, uint16_t time){
    if (!tree->nodes[parent].left){
        tree->nodes[parent].left = &tree->nodes[child];
        tree->nodes[parent].time_left = time;
    }
    else{
        tree->nodes[parent].right = &tree->nodes[child];
        tree->nodes[parent].time_right = time;
    }
}


uint32_t calculateMass(node_s* node){
    if (!node) return 0;
    uint32_t mass = 0;
    for (uint8_t i = 0; i < node->size; i++){
        mass += node->values[i];
    }
    return mass;
}

void calculateBalance(node_s* node, uint32_t* balance){
    if (!node) return;
    uint32_t mass = calculateMass(node);
    if (node->left){
        *balance += abs((int)mass - (int)calculateMass(node->left));
        calculateBalance(node->left, balance);
    }
    if (node->right){
        *balance += abs((int)mass - (int)calculateMass(node->right));
        calculateBalance(node->right, balance);
    }
}

void backtrack(tree_s* tree, uint16_t *values, uint8_t valueID, node_s* node){
    if (valueID == B){
        uint32_t currentBalance = 0;
        calculateBalance(tree->root, &currentBalance);
        if (currentBalance < bestBalance || bestBalance == 0) {
            bestBalance = currentBalance;
            bestTime = currentTime;
        }
        else if (currentBalance == bestBalance && currentTime < bestTime) bestTime = currentTime;
        return;
    }

    if (node->valueIDX){
        node->values[node->valueIDX] = values[valueID];
        node->valueIDX++;
        backtrack(tree, values, valueID+1, tree->root);
        node->valueIDX--;
        node->values[node->valueIDX] = 0;
        return;
    }

    if ((node->left && node->left->valueIDX && node->right && node->right->valueIDX)
        || (node->left && node->left->valueIDX && !node->right)
        || (node->right && node->right->valueIDX && !node->left)){
        node->values[node->valueIDX] = values[valueID];
        node->valueIDX++;
        backtrack(tree, values, valueID+1, tree->root);
        node->valueIDX--;
        node->values[node->valueIDX] = 0;
    }

    if (node->left && !node->valueIDX){
        currentTime += node->time_left;
        backtrack(tree, values, valueID, node->left);
        currentTime -= node->time_left;
    }
    
    if (node->right && !node->valueIDX){
        currentTime += node->time_right;
        backtrack(tree, values, valueID, node->right);
        currentTime -= node->time_right;
    }

    if (!node->left && !node->right){
        node->values[node->valueIDX] = values[valueID];
        node->valueIDX++;
        backtrack(tree, values, valueID+1, tree->root);
        node->valueIDX--;
        node->values[node->valueIDX] = 0;
        return;
    }
}

int main(){
    if (scanf("%hhu %hhu", &N, &B) != 2){
        fprintf(stderr, "ERROR: Couldn't read data\n");
        return 1;
    }
    uint16_t *values = (uint16_t*)safeAllocation(sizeof(uint16_t)*B);
    for (int i = 0; i < B; i++){
        if (scanf(" %hu", &values[i]) != 1){
            fprintf(stderr, "ERROR: Couldn't read data\n");
            return 1;
        }
    }

    tree_s* tree = createTree(N, B);
    uint8_t p1, p2;
    uint16_t p3;
    for (int i = 0; i < N-1; i++){
        if (scanf("%hhu %hhu %hu", &p1, &p2, &p3) != 3){
            fprintf(stderr, "ERROR: Couldn't read data\n");
            return 1;
        }
        connect(tree, p1, p2, p3);
    }
    backtrack(tree, values, 0, tree->root);
    printf("%d %d\n", bestBalance, bestTime);
    free(values);
    return 0;
}

