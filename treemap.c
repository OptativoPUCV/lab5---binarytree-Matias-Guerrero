#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree == NULL) return;

    TreeNode* nodoActual = tree->root;

    TreeNode* nuevoNodo = createTreeNode(key, value);

    if(nodoActual != NULL)
    {
        TreeNode* nodoPadre = NULL;

        while(nodoActual != NULL)
        {
            nodoPadre = nodoActual;

            if(is_equal(tree, nodoActual->pair->key, key))
            {
                return;
            }
            else if(tree->lower_than(key, nodoActual->pair->key))
            {
                nodoActual = nodoActual->left;
            }
            else
            {
                nodoActual = nodoActual->right;
            }
        }

        if(tree->lower_than(key, nodoPadre->pair->key))
        {
            nodoPadre->left = nuevoNodo;
        }
        else
        {
            nodoPadre->right = nuevoNodo;
        }

        nuevoNodo->parent = nodoPadre;
        tree->current = nuevoNodo;
    }
    else
    {
        tree->root = nuevoNodo;
        tree->current = nuevoNodo;
    }

}

TreeNode * minimum(TreeNode * x){

    if(x->left == NULL)
    {
        return x;
    }
    else
    {
        return minimum(x->left);
    }

}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node->left == NULL && node->right == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = NULL;
        }
        else
        {
            if(node->parent->left == node)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
        }
    }
    else if(node->left == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = node->right;
        }
        else
        {
            if(node->parent->left == node)
            {
                node->parent->left = node->right;
            }
            else
            {
                node->parent->right = node->right;
            }

            node->right->parent = node->parent;
        }
    }
    else if(node->right == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = node->left;
        }
        else
        {
            if(node->parent->left == node)
            {
                node->parent->left = node->left;
            }
            else
            {
                node->parent->right = node->left;
            }

            node->left->parent = node->parent;
        }
    }
    else
    {
        TreeNode* min = minimum(node->right);

        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;

        removeNode(tree, min);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree == NULL || tree->root == NULL) return NULL;

    TreeNode* nodoActual = tree->root;

    while(nodoActual != NULL){

        if(is_equal(tree, nodoActual->pair->key, key))
        {
            tree->current = nodoActual;
            return nodoActual->pair;
        }
        else if(tree->lower_than(key, nodoActual->pair->key))
        {
            nodoActual = nodoActual->left;
        }
        else
        {
            nodoActual = nodoActual->right;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if(tree == NULL || tree->root == NULL) return NULL;

    if(searchTreeMap(tree, key) != NULL)
    {
        return tree->current->pair;
    }
    else
    {
        TreeNode* nodoActual = tree->root;
        TreeNode* nodoAuxiliar = nodoActual;

        while(nodoActual != NULL)
        {
            if(tree->lower_than(key, nodoActual->pair->key))
            {
                nodoAuxiliar = nodoActual;
            }

            if(tree->lower_than(key, nodoActual->pair->key))
            {
                nodoActual = nodoActual->left;
            }
            else
            {
                nodoActual = nodoActual->right;
            }
        }

        if(tree->lower_than(nodoAuxiliar->pair->key, key))
        {
            return NULL;
        }

        return nodoAuxiliar->pair;
    }

}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->root == NULL) return NULL;

    TreeNode* min = minimum(tree->root);

    tree->current = min;

    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree){
    if(tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;
    
    TreeNode* nodoActual = tree->current;

    if(nodoActual->right != NULL)
    {
        nodoActual = minimum(nodoActual->right);
    }
    else
    {
        TreeNode* nodoPadre = nodoActual->parent;

        while(nodoPadre != NULL && nodoActual == nodoPadre->right)
        {
            nodoActual = nodoPadre;
            nodoPadre = nodoPadre->parent;
        }

        if(nodoPadre == NULL)
        {
            return NULL;
        }
        else
        {
            nodoActual = nodoPadre;
        }
    }

    tree->current = nodoActual;

    return tree->current->pair;
}
