#include <stdio.h>
#include <stdlib.h>

typedef struct node_s {
    int key;
    struct node_s *left;
    struct node_s *right;
} node_s;

node_s *root;

void *safeAllocation(int size){
    void *mem = malloc(size);
    if (!mem){
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

node_s *createNode(int key){
    node_s *node = (node_s*)safeAllocation(sizeof(node_s));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

node_s *insertNode(node_s *node, int key){
    if (node == NULL){
        return createNode(key);
    }

    if (key < node->key){
        node->left = insertNode(node->left, key);
    }
    else if (key > node->key){
        node->left = insertNode(node->right, key);
    }

    return node;
}

node_s *deleteNode(node_s *node, int key){
    
}

// node_s* minValueNode(node_s* node) {
//     node_s* current = node;

//     // Find the leftmost leaf node
//     while (current->left != NULL) {
//         current = current->left;
//     }

//     return current;
// }

// // Function to delete a key from the BST
// node_s* deleteNode(node_s* root, int key) {
//     if (root == NULL) {
//         return root;
//     }

//     // Recursively search for the node to be deleted
//     if (key < root->key) {
//         root->left = deleteNode(root->left, key);
//     } else if (key > root->key) {
//         root->right = deleteNode(root->right, key);
//     } else {
//         // Node with only one child or no child
//         if (root->left == NULL) {
//             node_s* temp = root->right;
//             free(root);
//             return temp;
//         } else if (root->right == NULL) {
//             node_s* temp = root->left;
//             free(root);
//             return temp;
//         }

//         // Node with two children: Get the inorder successor
//         node_s* temp = minValueNode(root->right);

//         // Copy the inorder successor's key to this node
//         root->key = temp->key;

//         // Delete the inorder successor
//         root->right = deleteNode(root->right, temp->key);
//     }

//     return root;
// }

int main(){
    int K, N;
    if (scanf("%d %d", &K, &N) != 2){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }

    root = NULL;
    int bareBranchesDeleted = 0;

    while (N >= 0){
        char operation;
        int key;
        if (scanf("%c %d", &operation, &key) != 2){
            fprintf(stderr, "ERROR: Couldn't read data!\n");
            exit(1);
        }

        switch (operation) {
            case('I'):
                root = insertNode(root, key);
                N--;
                break;
            case('D'):
                root = deleteNode(root, key);
                N--;
                break;
            default:
                break;
        }

    }

    printf("%d", bareBranchesDeleted);
    return 0;
}