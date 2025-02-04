#include <stdio.h>
#include <stdlib.h>

typedef struct node_s {
    int key;
    struct node_s* left;
    struct node_s* right;
} node_s;

node_s *root;

void *safeAllocation(int size) {
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

node_s* createNode(int key) {
    node_s* node = (node_s*)safeAllocation(sizeof(node_s));
    if (node != NULL) {
        node->key = key;
        node->left = node->right = NULL;
    }
    return node;
    
}

void traverseTree(node_s *node, int *K){

}

int depthEvent;

void insertNode(node_s** node, int key, int* K, int depth, int* bareBranches) {
    if (*node == NULL) {
        *node = createNode(key);
        return;
    }

    if (((*node)->left == NULL && (*node)->right == NULL) ||
        ((*node)->left == NULL && (*node)->right != NULL) ||
        ((*node)->right == NULL && (*node)->left != NULL)) {
        (*bareBranches)++;
    }

    if (key < (*node)->key) {
        insertNode(&(*node)->left, key, K, depth + 1, bareBranches);
    } else if (key > (*node)->key) {
        insertNode(&(*node)->right, key, K, depth + 1, bareBranches);
    }

    if (*bareBranches == (int)(2^(*K)) - 1) {
        depthEvent = depth;
        *bareBranches = 0;
    }
    if (depth == depthEvent - *K) {
        traverseTree((*node), K);
        *bareBranches = 1;
    }
}

void deleteNode(node_s **node, int key, int *K, int depth, int *bareBranches) {  //change

    if ((*node)->key == key) {
        node_s *leftChild = (*node)->left;
        node_s *rightChild = (*node)->right;

        free(*node);

        
        if (leftChild != NULL) {
            *node = leftChild;
        }
        if (rightChild != NULL) {
            if (*node == NULL) {
                *node = rightChild;
            } else {
                node_s *current = *node;
                while (current->left != NULL) {
                    current = current->left;
                }
                current->left = rightChild;
            }
        }

        if (((*node)->left == NULL && (*node)->right == NULL) 
            || ((*node)->left == NULL && (*node)->right != NULL)
            || ((*node)->right == NULL && (*node)->left != NULL)) bareBranches++;

        if (*bareBranches == (2^(*K)) - 1) {
            depthEvent = depth;
            *bareBranches = 0;
        }
        if (depth == depthEvent - *K) {
            traverseTree((*node), K);
            *bareBranches = 1;
        }
    }

    if (key < (*node)->key) {
        if ((*node)->left != NULL) {
            deleteNode(&(*node)->left, key, K, depth + 1, bareBranches);
        }
    } else if (key > (*node)->key) {
        if ((*node)->right != NULL) {
            deleteNode(&(*node)->right, key, K, depth + 1, bareBranches);
        }
    }

    // if (((*node)->left == NULL && (*node)->right == NULL) 
    // || ((*node)->left == NULL && (*node)->right != NULL)
    // || ((*node)->right == NULL && (*node)->left != NULL)) bareBranches++;

    // // if (key < (*node)->key) {
    // //     if ((*node)->left != NULL) {
    // //         deleteNode(&(*node)->left, key, K, depth + 1, bareBranches);
    // //     }
    // // }
    // // else if (key > (*node)->key){
    // //     if ((*node)->right != NULL) {
    // //         deleteNode(&(*node)->right, key, K, depth + 1, bareBranches);
    // //     }
    // // }

    // if (*bareBranches == (2^(*K)) - 1) dephtEvent = depth;
    // if (depth == dephtEvent - *K) {
    //     traverseTree((*node), K);
    //     *bareBranches = 1;
    // }
}

void freeTree(node_s *node)  {
    if (node == NULL) return;
    if (node->left != NULL) freeTree(node->left);
    if (node->right != NULL) freeTree(node->right);
    free(node);
    node = NULL; 
}


int main() {
    int K, N;
    if (scanf("%d %d", &K, &N) != 2) {
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }
    root = NULL;
    int bareBranchesDeleted = 0;

    for (int i = 0; i < N; i++) {
        char operation;
        int key;
        if (scanf(" %c %d", &operation, &key) != 2) {
            fprintf(stderr, "ERROR: Couldn't read data!\n");
            exit(1);
        }
        if (operation == 'I') {
            int bareBranches = 0;
            insertNode(&root, key, &K, 0, &bareBranches);
            bareBranchesDeleted += bareBranches;
        }
        else if (operation == 'D') {
            int bareBranches = 0;
            deleteNode(&root, key, &K, 0, &bareBranches);
            bareBranchesDeleted += bareBranches;
        }

    }

    printf("%d\n", bareBranchesDeleted);

    freeTree(root);
    free(root);
    root = NULL;

    return 0;
}
