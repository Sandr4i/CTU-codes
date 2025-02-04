#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(a,b) ((a) >= (b) ? (a) : (b))

int N;
int D = 0, R = 0, C = 0;

typedef struct node_s {
    int key;
    _Bool delete;
    int height;
    int deleteCount;
    struct node_s *left;
    struct node_s *right;
} node_s;

node_s *root;

int height(node_s *node){
    if (node == NULL) return 0;
    return node->height;
}

int deleteCount(node_s *node){
    if (node == NULL) return 0;
    return node->deleteCount;
}

int getBalance(node_s *node){
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

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
    node->height = 1;
    node->deleteCount = 0;
    node->delete = false;
    node->right = NULL;
    node->left = NULL;
    return node;
}

int maxDepth(node_s *node){
    if (node == NULL) return -1;
    int leftDepth = maxDepth(node->left);
    int rightDepth = maxDepth(node->right);
    return 1 + MAX(leftDepth, rightDepth);
}

node_s *rightRotation(node_s *node){
    node_s *x = node->left;
    node_s *temp = x->right;

    x->right = node;
    node->left = temp;

    node->height = MAX(height(node->left), height(node->right)) + 1;
    node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right)) + node->delete;
    x->height = MAX(height(x->left), height(x->right)) + 1;
    x->deleteCount = MAX(deleteCount(x->left), deleteCount(x->right)) + x->delete;
    return x;
}

node_s *leftRotation(node_s *node){
    node_s *x = node->right;
    node_s *temp = x->left;

    x->left = node;
    node->right = temp;

    node->height = MAX(height(node->left), height(node->right)) + 1;
    node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right)) + node->delete;
    x->height = MAX(height(x->left), height(x->right)) + 1;
    x->deleteCount = MAX(deleteCount(x->left), deleteCount(x->right)) + x->delete;
    return x;
}

node_s *rotate(node_s *node){
    if (node->left->height)
}

node_s *insert(node_s *node, int key){
    if (node == NULL){
        node = createNode(key);
        return node;
    }

    if (key < node->key){ 
        node->left = insert(node->left, key);
    }
    else if (key > node->key){
        node->right = insert(node->right, key);
    }
    else{
        if (node->delete) node->deleteCount--;
        node->delete = false;
    }

    node->height = 1 + MAX(height(node->left), height(node->right));
    node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right));
    if (node->delete) node->deleteCount++;

    int balance  = getBalance(node);

    if (balance > 1 && key < node->left->key){
        R++;
        return rightRotation(node);
    }

    if (balance < -1 && key > node->right->key){
        R++;
        return leftRotation(node);
    }

    if (balance > 1 && key > node->left->key){
        R+=2;
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    if (balance < -1 && key < node->right->key){
        R+=2;
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }
    return node;
}

node_s *delete(node_s *node, int key){
    if (node == NULL) return node;

    if (key < node->key) node->left = delete(node->left, key);
    else if (key > node->key) node->right = delete(node->right, key);
    else{
        node->delete = true;
    }
    node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right));
    if (node->delete) node->deleteCount++;
    
    return node;
}

node_s *getNodeToRemove(node_s *node) {
    if (node == NULL) return NULL;

    node_s *temp = NULL;
    if(node->left){
        if (node->left->deleteCount > 0){
            temp = getNodeToRemove(node->left);
        }
    }
    
    if (temp) return temp;

    if (node->right){
        if (node->right->deleteCount > 0){
            temp = getNodeToRemove(node->right);
        }
    }

    if (temp) return temp;

    if (node->delete) return node;
    return NULL;
}

node_s *minValueNode(node_s* node) 
{ 
    node_s* current = node; 
 
    while (current->left != NULL){
        current = current->left; 
    }
    return current; 
} 

node_s *removeNode(node_s *node, node_s *nodeToRemove){
    if (node == NULL) return NULL;
    if (nodeToRemove->key < node->key){
        node->left = removeNode(node->left, nodeToRemove);
    }
    else if (nodeToRemove->key > node->key){
        node->right = removeNode(node->right, nodeToRemove);
    }
    else {
        if (node->left == NULL && node->right == NULL){
            free(node);
            return NULL;
        }
        else if (node->left){
            node_s *temp = root->left;
            while(temp->right){
                temp = temp->right;
            }
            node->key = temp->key;
            node->delete = false;
            node->left = removeNode(node->left, temp);
        }
        else {
            node_s *temp = node->right;
            while(temp->left){
                temp = temp->left;
            }
            node->key = temp->key;
            node->delete = false;
            node->right = removeNode(node->right, temp);
        }
    }
    node->height = MAX(height(node->left), height(node->right)) + 1;
    node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right)) + node->delete;
    return rotate(node);

}

// node_s *removeNode(node_s *node, node_s *nodeToRemove){
//     if (node == NULL) return node;

//     if (nodeToRemove->key < node->key){
//         node->left = removeNode(node->left, nodeToRemove);
//     }
//     else if (nodeToRemove->key > node->key){
//         node->right = removeNode(node->right, nodeToRemove);
//     }
//     else {
//         if (node->left == NULL || node->right == NULL){
//             node_s *temp = node->left ? node->left : node->right;
//             if (temp == NULL){
//                 temp = node;
//                 node = NULL;
//             }
//             else *node = *temp;
//             free(temp);
//         }
//         else {
//             node_s *temp = minValueNode(node->right);
//             node->key = temp->key;
//             node->delete = temp->delete;
//             node->right = removeNode(node->right, temp);
//         }
//     }
//     if (node == NULL) return node;

//     node->height = 1 + MAX(height(node->left), height(node->right));
//     node->deleteCount = MAX(deleteCount(node->left), deleteCount(node->right));
//     if (node->deleteCount) node->deleteCount++;

//     int balance = getBalance(node);
//     if (balance > 1 && getBalance(node->left) >= 0) {
//         R++;
//         return rightRotation(node);
//     }
//     if (balance > 1 && getBalance(node->left) <= 0){
//         R+=2;
//         node->left = leftRotation(node->left);
//         return rightRotation(node);
//     }
//     if (balance < -1 && getBalance(node->right) <= 0){
//         R++;
//         return leftRotation(node);
//     }
//     if (balance < -1 && getBalance(node->right) > 0){
//         R+=2;
//         node->right = rightRotation(node->right);
//         return leftRotation(node);
//     }
//     return node;
// }


void printfTree(node_s *node, int layer){
    if (node == NULL) return;
    printfTree(node->left, layer+1);
    printf("node: %d; layer: %d\n", node->key, layer);
    printf("Delete: %d\n", node->delete);
    printfTree(node->right, layer + 1);
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
            root = insert(root, abs(key));
        }
        else {
            root = delete(root, abs(key));
        }
        // Konsolidace
        if (root->deleteCount >= (1+floor((float)root->height/2))){
            C++;
            node_s *nodeToRemove = getNodeToRemove(root);
            while (nodeToRemove){
                //printf("%d\n", nodeToRemove->key);
                root = removeNode(root, nodeToRemove);
                nodeToRemove = getNodeToRemove(root);
            }
        }
    }
    // printfTree(root, 0);
    if (root == NULL) D = -1;
    else D = MAX(height(root->left), height(root->right));
    printf("%d %d %d\n", D, R, C);
}