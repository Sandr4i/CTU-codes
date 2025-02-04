#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int K = 0;
int RESULT = 0;
#define CHAIN_SIZE (pow(2, K) - 1)

typedef struct node_s {
    int value;
    struct node_s* left;
    struct node_s* right;
} node_s;

typedef node_s* NodeObject;
typedef NodeObject* NodeObjectReference;
typedef NodeObject* NodeObjectsArray;
typedef NodeObjectReference* NodeObjectReferencesArray;

typedef struct tree_s {
    NodeObject root;
} tree_s;

typedef tree_s* TreeObject;

/// Init

void *safeAlloc(unsigned long size){
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "ERROR: Allocation failed!\n");
        exit(2);
    }
    return mem;
}

NodeObject createNode(int value, NodeObject left, NodeObject right) {
    NodeObject node = (NodeObject)safeAlloc(sizeof(node_s));
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}

NodeObject createLeafNode(int value) {
    return createNode(value, NULL, NULL);
}

void initEmptyTree(TreeObject tree) {
    tree->root = NULL;
}

void initTree(TreeObject tree, int value) {
    tree->root = createLeafNode(value);
}

void insertNodeToTree(TreeObject tree, NodeObject node);

void insertValueToTree(TreeObject tree, int value);

/// Insert

void insertNode(NodeObject currentNode, NodeObject nodeToInsert);

void tryToInsert(NodeObjectReference currentNode, NodeObject nodeToInsert) {
    if (!*currentNode) {
        *currentNode = nodeToInsert;
    } else {
        insertNode(*currentNode, nodeToInsert);
    }
}

void insertNode(NodeObject currentNode, NodeObject nodeToInsert) {
    if (nodeToInsert->value < currentNode->value) {
        tryToInsert(&currentNode->left, nodeToInsert);
    } else {
        tryToInsert(&currentNode->right, nodeToInsert);
    }
}

void insertNodeToTree(TreeObject tree, NodeObject node) {
    if (tree->root) {
        insertNode(tree->root, node);
    } else {
        tree->root = node;
    }
}

void insertValueToTree(TreeObject tree, int value) {
    insertNodeToTree(tree, createLeafNode(value));
}

NodeObjectReference mostLeft(NodeObjectReference currentNode) {
    if (!(*currentNode)->left) {
        return currentNode;
    } else {
        return mostLeft(&(*currentNode)->left);
    }
}

void modify(NodeObject node, int value, NodeObject left, NodeObject right) {
    if (node->value == value) {
        node->left = left;
        node->right = right;
        return;
    }
    if (node->left) {
        modify(node->left, value, left, right);
    }
    if (node->right) {
        modify(node->right, value, left, right);
    }
}

/// Delete

void destroyNode(NodeObjectReference node) {
    free(*node);
    *node = NULL;
}

int tryToDelete(NodeObjectReference currentNode, int value) {
    if (*currentNode && (*currentNode)->value == value) {
        if (!(*currentNode)->left && !(*currentNode)->right) {
            destroyNode(currentNode);
        } else if (!(*currentNode)->left && (*currentNode)->right) {
            NodeObjectReference tmp = currentNode;
            *currentNode = (*currentNode)->right;
            destroyNode(tmp);
        } else if ((*currentNode)->left && !(*currentNode)->right) {
            NodeObjectReference tmp = currentNode;
            *currentNode = (*currentNode)->left;
            destroyNode(tmp);
        } else {
            node_s** mostLeftOfRight = mostLeft(&(*currentNode)->right);
            (*currentNode)->value = (*mostLeftOfRight)->value;
            destroyNode(mostLeftOfRight);
        }
        return 0;
    } else {
        return 1;
    }
}

void deleteNode(NodeObjectReference currentNode, int value) {
    if (tryToDelete(currentNode, value)) {
        if ((*currentNode)->left) {
            deleteNode(&(*currentNode)->left, value);
        }
        if ((*currentNode)->right) {
            deleteNode(&(*currentNode)->right, value);
        }
    }
}

void deleteNodeFromTree(TreeObject tree, int value) {
    deleteNode(&tree->root, value);
}

/// Chains

int findChain(NodeObject currentNode, NodeObjectsArray currentChain, int currentSize) {
    if (currentSize == CHAIN_SIZE) {
        return currentSize;
    }
    currentChain[currentSize] = currentNode;
    if (currentNode && currentNode->left && !currentNode->right) {
        return findChain(currentNode->left, currentChain, currentSize + 1);
    } else if (currentNode && !currentNode->left && currentNode->right) {
        return findChain(currentNode->right, currentChain, currentSize + 1);
    } else {
        return currentSize + 1;
    }
}

void printTree(TreeObject tree);

void sortNodes(NodeObjectsArray nodes, int nodesCount);

void buildPerfectTree(TreeObject tree, NodeObjectsArray nodes, int nodesCount);

NodeObject findChains(NodeObject node) {
    NodeObjectsArray chain = malloc(CHAIN_SIZE * sizeof(NodeObject));
    int resultSize = findChain(node, chain, 0);
    if (resultSize >= CHAIN_SIZE) {
        tree_s newTree;
        buildPerfectTree(&newTree, chain, resultSize);
        free(chain);
        RESULT++;
        return newTree.root;
    } else {
        free(chain);
        return NULL;
    }
}

void findChainForNode(NodeObjectReference node) {
    if ((*node)->left) {
        findChainForNode(&(*node)->left);
    }
    if ((*node)->right) {
        findChainForNode(&(*node)->right);
    }
    NodeObject result = findChains(*node);
    if (result) {
        *node = result;
    }
}

void findChainInTree(TreeObject tree) {
    findChainForNode(&(tree->root));
}

/// Print

void printNode(NodeObject node, int level);

void printBranch(NodeObject node, int level, const char* text) {
    if (node) {
        printf("\n");
        for (int i = 0; i < level; ++i) {
            printf(" ");
        }
        printf("%s", text);
        printNode(node, level + 1);
    }
}

void printNode(NodeObject node, int level) {
    printf("%d", node->value);
    printBranch(node->left, level, "Left branch ");
    printBranch(node->right, level, "Right branch ");
}

void printTree(TreeObject tree) {
    printNode(tree->root, 0);
    printf("\n");
}

/// Perfect tree

void sortNodes(NodeObjectsArray nodes, int nodesCount) {
    for (int i = 0; i < nodesCount; ++i) {
        for (int j = i; j < nodesCount; ++j) {
            if (nodes[i]->value > nodes[j]->value) {
                NodeObject tmp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = tmp;
            }
        }
    }
}

NodeObject buildPerfectSubnodes(NodeObject currentNode, NodeObjectsArray nodes, int from, int to, NodeObject lastNode) {
    int resultIndex = (to + from)/2;
    NodeObject nextNode = nodes[resultIndex];
    NodeObject result;
    if (nextNode->value == lastNode->value) {
        result = nextNode;
    } else {
        result = createLeafNode(nextNode->value);
        free(nextNode);
    }
    if (from < resultIndex && to - from > 2) {
        result->left = buildPerfectSubnodes(result, nodes, from, resultIndex, lastNode);
    }
    if (resultIndex < to - 1) {
        result->right = buildPerfectSubnodes(result, nodes, resultIndex, to, lastNode);
    }
    return result;
}

void buildPerfectTree(TreeObject tree, NodeObjectsArray nodes, int nodesCount) {
    NodeObject last = nodes[nodesCount - 1];
    sortNodes(nodes, nodesCount);
    tree->root = buildPerfectSubnodes(tree->root, nodes, 0, nodesCount, last);
}

int main() {
    RESULT = 0;
    tree_s tree;
    initEmptyTree(&tree);

    int n;
    if (scanf("%d %d", &K, &n) != 2){
        fprintf(stderr, "ERROR: Couldn't read data!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        char command[1];
        int value;
        if (scanf("%s %d", command, &value) != 2) {
            fprintf(stderr, "ERROR: Couldn't read data!\n");
            exit(1);
        }
        if (!strcmp(command, "I")) {
            insertValueToTree(&tree, value);
        } else if (!strcmp(command, "D")) {
            deleteNodeFromTree(&tree, value);
        }
        findChainInTree(&tree);
    }
    printf("%d\n", RESULT);
    return 0;
}
