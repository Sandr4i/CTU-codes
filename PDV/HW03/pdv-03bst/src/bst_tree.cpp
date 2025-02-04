#include "bst_tree.h"
#include <atomic>

void bst_tree::insert(int64_t data) {
    node* new_node = new node(data);

    node* null_node = nullptr;
    if (root == nullptr){
        if(reinterpret_cast<std::atomic<node*>*>(&root)->compare_exchange_strong(null_node, new_node)) return;
    }

    node* current = root;
    while(true){
        if (current->data > data){
            if (current->left != nullptr) current = current->left;
            else if(reinterpret_cast<std::atomic<node*>*>(&current->left)->compare_exchange_strong(null_node, new_node)) break;
        }
        else{
            if (current->right != nullptr) current = current->right;
            else if(reinterpret_cast<std::atomic<node*>*>(&current->right)->compare_exchange_strong(null_node, new_node)) break;
        }
    }
}

// Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym uzlum
void delete_node(bst_tree::node* node) {
    if (node == nullptr) {
        return;
    }

    delete_node(node->left);
    delete_node(node->right);
    delete node;
}

bst_tree::~bst_tree() {
    delete_node(root);
}
