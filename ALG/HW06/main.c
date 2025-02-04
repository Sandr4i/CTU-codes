#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (a > b ? a : b)
#define HEIGHT(node) (node ? node->height : -1)
#define DELETE_COUNT(node) (node ? node->deleteCount : 0)

typedef struct node_s {
    int key;
    int delete;
    int height;
    int deleteCount;
    struct node_s *left;
    struct node_s *right;
} node_s;

node_s *root;

int N;
int D = 0, R = 0, C = 0;

void *safeAlloc(unsigned long size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

node_s *createNode(int key){
    node_s *node = (node_s*)safeAlloc(sizeof(node_s));
    node->key = key;
    node->delete = 0;
    node->height = 0;
    node->deleteCount = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}



void getHeight(node_s *node){
    node->height = MAX(HEIGHT(node->left), HEIGHT(node->right)) + 1;
}

void getDeleteCount(node_s *node){
    node->deleteCount = MAX(DELETE_COUNT(node->left), DELETE_COUNT(node->right)) + node->delete;
}

node_s *rightRotation(node_s *node){
    R++;

    node_s *x = node->left;
    node->left = x->right;
    x->right = node;

    getHeight(node);
    getHeight(x);

    getDeleteCount(node);
    getDeleteCount(x);
    return x;
}

node_s *leftRotation(node_s *node){
    R++;

    node_s *x = node->right;
    node->right = x->left;
    x->left = node;

    getHeight(node);
    getHeight(x);

    getDeleteCount(node);
    getDeleteCount(x);

    return x;
}

node_s *rightLeftRotation(node_s *node){
    node->right = rightRotation(node->right);
    return leftRotation(node);
}

node_s *leftRightRotation(node_s *node){
    node->left = leftRotation(node->left);
    return rightRotation(node);
}

node_s *rotate(node_s *node){
    if (HEIGHT(node->left) - HEIGHT(node->right) > 1){
        if (HEIGHT(node->left->left) >= HEIGHT(node->left->right)){
            return rightRotation(node);
        }
        else return leftRightRotation(node);
    }
    else if (HEIGHT(node->right) - HEIGHT(node->left) > 1){
        if (HEIGHT(node->right->right) >= HEIGHT(node->right->left)){
            return leftRotation(node);
        }
        else return rightLeftRotation(node);
    }
    return node;
}

node_s *insert(node_s *node, int key){
    if (node == NULL){
        return createNode(key);
    }

    if (key < node->key){
        node->left = insert(node->left, key);
    }
    else if (key > node->key){
        node->right = insert(node->right, key);
    }
    else node->delete = 0;

    getHeight(node);
    getDeleteCount(node);
    return rotate(node);
}

node_s *delete(node_s *node, int key){
    if (node == NULL) return node;

    if (key < node->key){
        node->left = delete(node->left, key);
    }
    else if (key > node->key){
        node->right = delete(node->right, key);
    }
    else {
        node->delete = 1;
    }

    getHeight(node);
    getDeleteCount(node);
    return node;
}

node_s *getNodeToRemove(node_s *node){
    if (node == NULL) return NULL;

    node_s *temp = NULL;
    if (DELETE_COUNT(node->left)){
        temp = getNodeToRemove(node->left);
    }
    if (temp != NULL) return temp;

    if (DELETE_COUNT(node->right)){
        temp = getNodeToRemove(node->right);
    }
    if (temp != NULL) return temp;

    if (node->delete) return node;

    return NULL;
}

node_s *removeNode(node_s *node, int key){
    if (node == NULL) return NULL;

    if (key < node->key){
        node->left = removeNode(node->left, key);
    }
    else if (key > node->key){
        node->right = removeNode(node->right, key);
    }
    else {
        // If node is a leaf
        if (node->left == NULL && node->right == NULL){
            free(node);
            return NULL;
        }
        // If node has 1 childe - left
        else if (node->left){
            node_s *temp = node->left;
            while(temp->right){
                temp = temp->right;
            }
            node->key = temp->key;
            node->delete = 0;
            node->left = removeNode(node->left, temp->key);
        }
        // If node has 1 childe - right
        else {
            node_s *temp = node->right;
            while(temp->left){
                temp = temp->left;
            }
            node->key = temp->key;
            node->delete = 0;
            node->right = removeNode(node->right, temp->key);
        }
    }
    getHeight(node);
    getDeleteCount(node);

    return rotate(node);
}

void freeNode(node_s *node){
    if (!node){
        freeNode(node->left);
        node->left = NULL;
        freeNode(node->right);
        node->right = NULL;
        free(node);
        node = NULL;
    }
}

void freeTree(node_s *root){
    if (root) freeNode(root);
    root = NULL;
}

void printTree(node_s *node, int layer){
    if (node == NULL) return;
    printTree(node->left, layer+1);
    printf("node: %d; layer: %d\n", node->key, layer);
    printf("Delete: %d\n", node->delete);
    printTree(node->right, layer + 1);
}

int main(){
    root = NULL;

    if (scanf("%d", &N) != 1){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }

    for (int i = 0; i < N; i++){
        int key;

        if (scanf("%d", &key) != 1){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
        }
        if (key > 0){
            root = insert(root, key);
        }
        else {
            root = delete(root, abs(key));
        }
        // Konsolidace
        if (root && (root->deleteCount >= 1 + (HEIGHT(root)/2))) {
            C++;
            node_s *nodeToRemove = getNodeToRemove(root);
            while(nodeToRemove){
                root = removeNode(root, nodeToRemove->key);
                nodeToRemove = getNodeToRemove(root);
            }
        }
    }
    //printTree(root, 0);
    D = HEIGHT(root);
    printf("%d %d %d\n", D, R, C);
    return 0;
}