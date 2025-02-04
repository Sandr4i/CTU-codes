#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int K = 0;
int result = 0;

#define CHAIN_SIZE ((int)pow(2, K) - 1)

typedef struct node_s {
    int value;
    struct node_s* parent;
    struct node_s* left;
    struct node_s* right;
} node_s;

node_s *root;

//Bare branch root
node_s* chain = NULL;
//Used as couner of nodes in bare branche
int chainSize = 0;

//Node used as a starting point to search for bare branch
node_s* start;

void *safeAlloc(unsigned long size);

node_s* createNode(int value, node_s *parent);

node_s* insertNode(node_s *node, int value, node_s *parent);

int minValue(node_s *node);

node_s* deleteNode(node_s *node, int value);

void searchForChainInsert(node_s* node);

void searchForChainDelete(node_s* node);

void traverseTree();

void printfTree(node_s *node);

int main(){
    clock_t start_time = clock();
    result = 0;
    root = NULL;

    int N;
    if (scanf("%d %d", &K, &N) != 2){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }
    while (N > 0){
        char operation[1];
        int value;
        if (scanf("%s %d", operation, &value) != 2){
            fprintf(stderr, "ERROR: Couldn't read data!\n");
            exit(1);
        }
        // chain = (node_s*)safeAlloc(sizeof(node_s));
        chainSize = 0;
        printf("\n---------------\nIteration: %d\n\n", N);
        switch (operation[0]) {
            case('I'):
                root = insertNode(root, value, NULL);
                if (start->parent != NULL) searchForChainInsert(start);
                if (chainSize > CHAIN_SIZE){
                    //traverseTree();
                    result++;
                }
                break;
            case('D'):
                root = deleteNode(root, value);
                if (root == NULL)printf("Deleted\n");
                if (root != NULL){
                    if (root->left != NULL && root->right != NULL){
                        if (value < root->value) searchForChainDelete(root->left);
                        else searchForChainDelete(root->right);
                    }
                    else searchForChainDelete(root);
                }
                break;
            default:
                break;
        }
        //printf("Value: %d; Start: %d\n",value, start->value);
        printf("Tree:\n");
        printfTree(root);
        N--;
        free(chain);
        chain = NULL;
        start = NULL;
        if (N > 0) chainSize = 0;

    }
    printf("\nChain size: %d\n", chainSize);
    //printfTree(root);
    printf("\n%d\n", result);
    
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time: %.6f\n", elapsed_time);

    return 0;
}

void *safeAlloc(unsigned long size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

node_s* createNode(int value, node_s *parent){
    node_s* node = (node_s*)safeAlloc(sizeof(node_s));
    node->value = value;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    return node;
}

node_s* insertNode(node_s *node, int value, node_s *parent){
    if (node == NULL){
        node = createNode(value, parent);
        start = node;
    }

    if (value < node->value){
        node->left = insertNode(node->left, value, node);
    }
    else if (value > node->value){
        node->right = insertNode(node->right, value, node);
    }

    return node;
}

int minValue(node_s *node){
    while(node->left != NULL){
        node = node->left;
    }
    return node->value;
}

node_s* deleteNode(node_s *node, int value){
    if (node == NULL) return node;
    if (node->value > value){
        node->left = deleteNode(node->left, value);
        if (node->left) node->left->parent = node;
        return node;
    }
    else if (node->value < value){
        node->right = deleteNode(node->right, value);
        if (node->right) node->right->parent = node;
        return node;
    }
    if (node->left == NULL){
        node_s *temp = node->right;
        if (temp){
            temp->parent = node->parent;
            // start = temp->parent;
        }
        free(node);
        return temp;
    }
    else if (node->right == NULL){
        node_s *temp = node->left;
        if (temp) {
            temp->parent = node->parent;
            //start = temp->parent;
        }
        free(node);
        return temp;
    }
    else{
        node_s* successorParent = node;
        node_s *successor = node->right;
        while (successor->left != NULL){
            successorParent = successor;
            successor = successor->left;
        }
        if (successorParent != node){
            successorParent->left = successor->right;
        }
        else{
            successorParent->right = successor->right;
        }
        node->value = successor->value;
        node->parent = successor->parent;
        if (successor->right){
            successor->right->parent = successorParent;
        }
        free(successor);
        //start = node->parent;
        return node;
    }
}

void searchForChainInsert(node_s* node){
    if (node->right != NULL && node->left != NULL) return;

    if ((node->left == NULL && node->right != NULL) 
    || (node->left != NULL && node->right == NULL)
    || (node->left == NULL && node->right == NULL)) {
        if (chainSize == 0){
            chain = node;
        }
        chainSize++;
        printf("Chain element: %d\n", node->value);
        printf("id: %d\n", chainSize);
    }
    else{
        printf("Node with 2 children: %d\n", node->value);
        printf("Left child: %d, Right childe: %d\n", node->left->value, node->right->value);
    }
    if (node->parent != NULL) searchForChainInsert(node->parent);
}

void searchForChainDelete(node_s* node){
    if (node == NULL) return;
    // Chain 
    if ((node->left == NULL && node->right != NULL) 
    || (node->left != NULL && node->right == NULL)
    || (node->left == NULL && node->right == NULL)){
        if (chainSize == 0){
            chain = node;
        }
        chainSize++;
        printf("Chain element: %d\n", node->value);
        printf("id: %d\n", chainSize);
    }
    else{
        printf("Node with 2 children: %d\n", node->value);
        printf("Left child: %d, Right childe: %d\n", node->left->value, node->right->value);
    }
    // Chain
    if (node->left != NULL && node->right != NULL) return;
    else {
        if (node->left != NULL && node->right == NULL) searchForChainDelete(node->left);
        if (node->left == NULL && node->right != NULL) searchForChainDelete(node->right);
    }

}

void traverseTree(){

}

void printfTree(node_s *node){
    if (node == NULL) return;
    printfTree(node->left);
    if (node->parent) printf("node: %d parent: %d; ", node->value, node->parent->value);
    else printf("node: %d; ", node->value);
    printfTree(node->right);
}




/*
Old funcions
*/

// if (chainID < size){
        //     chain[chainID++] = node;
            // printf("Chain element: %d\n", node->value);
            // printf("id: %d\n", chainID);
        // }
        // else{
        //     size *= 2;
        //     printf("size: %d(realloc)\n", size);
        //     node_s** newChain = realloc(chain, sizeof(node_s) * size);
        //     if (newChain == NULL){
        //         fprintf(stderr, "ERROR: Reallocation failed!\n");
        //         exit(2);
        //     }
        //     chain = newChain;
        //     chain[chainID++] = node;
        //     printf("Chain element: %d\n", node->value);
        //     printf("id: %d\n", chainID);
        // }

// void searchForChain(node_s* node){ //change: start from bottom
//     if (node == NULL) return;
//     // Chain 
//     if ((node->left == NULL && node->right != NULL) 
//     || (node->left != NULL && node->right == NULL)
//     || (node->left == NULL && node->right == NULL)){
//         if (chainID < size){
//             chain[chainID++] = node;
//             printf("Chain element: %d\n", node->value);
//             printf("id: %d\n", chainID);
//         }
//         else{
//             size *= 2;
//             printf("size: %d(realloc)\n", size);
//             node_s** newChain = realloc(chain, sizeof(node_s) * size);
//             if (newChain == NULL){
//                 fprintf(stderr, "ERROR: Reallocation failed!\n");
//                 exit(2);
//             }
//             chain = newChain;
//             chain[chainID++] = node;
//             printf("Chain element: %d\n", node->value);
//             printf("id: %d\n", chainID);
//         }
//     }
//     else{
//         printf("Node with 2 children: %d\n", node->value);
//         printf("Left child: %d, Right childe: %d\n", node->left->value, node->right->value);
//     }
//     // Chain
//     if (node->left != NULL && node->right != NULL) return;
//     else {
//         if (node->left != NULL && node->right == NULL) searchForChain(node->left);
//         if (node->left == NULL && node->right != NULL)searchForChain(node->right);
//     }
// }

// node_s* deleteNode(node_s *node, int value){
//     if (node == NULL) return node;
//     if (value < node->value){
//         node->left = deleteNode(node->left, value);
//     }
//     else if (value > node->value){
//         node->right = deleteNode(node->right, value);
//     }
//     else {
//         if (node->left == NULL){
//             // node_s *temp = node->right;
//             // if (temp) temp->parent = node;
//             // free(node);
//             // return temp;
//             return node->right;
//         }
//         else if (node->right == NULL){
//             // node_s *temp = node->left;
//             // if (temp) temp->parent = node;
//             // free(node);
//             // return temp;
//             return node->left;
//         }
//         else {
//             node->value = minValue(node->right);
//             node->right = deleteNode(node->right, node->value);
//         }
//         return node;
//     }
//     return node;
// }


// node_s* findSmallestNode(node_s* node, node_s* smallest, int value){
//     if (node->value < value){
//         value = node->value;
//         smallest = node;
//     }
//     if (node->left != NULL){
//         findSmallestNode(node->left, smallest, value);
//     }

//     return smallest;
// }
// node_s* deleteNode(node_s *node, int value){
//     if (node->value == value){
//         if (node->left == NULL && node->right == NULL){
//             free(node);
//             node = NULL;
//         }
//         else if (node->left != NULL && node->right == NULL){
//             node = node->left;
//         }
//         else if (node->left == NULL && node->right != NULL){
//             node = node->right;
//         }
//         else if (node->left != NULL && node->right != NULL){
//             node_s* smallest = findSmallestNode(node, node, node->value);
//             node->value = smallest->value;
//             smallest = smallest->right;
//         }
//         return node;
//     }

//     if (value < node->value){
//         deleteNode(node->left, value);
//     }
//     else if (value > node->value){
//         deleteNode(node->right, value);
//     }
//     return node;
// }
