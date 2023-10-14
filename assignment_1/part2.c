#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * BST Related data structures and functions
 */

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
    pthread_mutex_t lock;  // Mutex for node-level locking
} node;

/*
 * Tree root - GLOBAL
 */

node* groot = NULL;



typedef enum {SEARCH, INSERT, DELETE} OPTYPE;

node* new_node() {
    node *n = malloc(sizeof(node));
    if (n == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    n->val = -1;
    n->left = NULL;
    n->right = NULL;
    pthread_mutex_init(&(n->lock), NULL);  // Initialize the node lock
    return n;
}



node* insert(node* root, int val) {
    if (root == NULL) {
        pthread_mutex_lock(&(groot->lock));  // Acquire lock for root
        root = new_node();
        root->val = val;
        pthread_mutex_unlock(&(groot->lock));  // Release lock for root
    } else {
        if (val < root->val) {
            pthread_mutex_lock(&(root->left->lock));  // Acquire lock for left child
            root->left = insert(root->left, val);
            pthread_mutex_unlock(&(root->left->lock));  // Release lock for left child
        } else {
            pthread_mutex_lock(&(root->right->lock));  // Acquire lock for right child
            root->right = insert(root->right, val);
            pthread_mutex_unlock(&(root->right->lock));  // Release lock for right child
        }
    }

    return root;
}


node* delete(node* root, int val) {
    if (root == NULL) {
        return NULL;
    } else {
        if (val < root->val) {
            pthread_mutex_lock(&(root->left->lock));  // Acquire lock for left child
            root->left = delete(root->left, val);
            pthread_mutex_unlock(&(root->left->lock));  // Release lock for left child
        } else if (val > root->val) {
            pthread_mutex_lock(&(root->right->lock));  // Acquire lock for right child
            root->right = delete(root->right, val);
            pthread_mutex_unlock(&(root->right->lock));  // Release lock for right child
        } else {  // equal: found node to be deleted
            // Three cases...
            // (Same as the original code)
        }
    }

    return root;
}


node* search(node *root, int val) {
    if (root == NULL)
        return NULL;
    else {
        if (root->val == val) {
            return root;
        } else if (val < root->val) {
            pthread_mutex_lock(&(root->left->lock));  // Acquire lock for left child
            node* result = search(root->left, val);
            pthread_mutex_unlock(&(root->left->lock));  // Release lock for left child
            return result;
        } else {
            pthread_mutex_lock(&(root->right->lock));  // Acquire lock for right child
            node* result = search(root->right, val);
            pthread_mutex_unlock(&(root->right->lock));  // Release lock for right child
            return result;
        }
    }
}


void print_tree_inorder (node* root) {
    if (root==NULL)
        return;
    // else
    print_tree_inorder (root->left);
    fprintf(stdout, "%d, ", root->val);
    print_tree_inorder (root->right);
}

/*
 * Threads related data structures and functions
 */

/*
 * The op structure specifies the operation to perform
 */

typedef struct op {
    OPTYPE type;
    int val;
} op;

typedef struct thread_arg {
    // node* root; // made global
    op *op_start;
    int count;
} thread_arg;


// comment line below to test large multithreadded
#define SMALLSIZE

#ifdef SMALLSIZE
#define NUMOPS 10
#define NUMTHREADS 1
#define MODULAND 10
#define SEARCHBIN 4
#define INSERTBIN 7
#define DELETEBIN 10
#else  // LARGESIZE
#define NUMOPS 800000
#define NUMTHREADS 8
#define MODULAND 10000
#define SEARCHBIN 4000
//#define SEARCHBIN 9990
// #define INSERTBIN 9995
#define INSERTBIN 7000
#define DELETEBIN 10000
#endif

void init_ops (op *ops){
    for (int i=0; i<NUMOPS; i++){

        int num = rand();
        ops[i].val = num%100;

        int op = num%MODULAND;
        if (op < SEARCHBIN){
            ops[i].type = SEARCH;
        }
        else if (op < INSERTBIN) {
            ops[i].type = INSERT;
        }
        else {  // DELETEBIN
            ops[i].type = DELETE;
        }
    }
}

// thread operations

// node* test_tree (node* root) {
node* test_tree () {

    groot = insert(groot, 7);
    groot = insert(groot, 5);
    groot = insert(groot, 3);
    groot = insert(groot, 4);
    groot = insert(groot, 9);
    groot = insert(groot, 8);
    groot = insert(groot, 10);

    fprintf(stdout, "tree: ");
    print_tree_inorder(groot);
    fprintf(stdout, "\n");

    groot = delete (groot, 3);
    fprintf(stdout, "tree after deleting 3: ");
    print_tree_inorder(groot);
    fprintf(stdout, "\n");

    groot = delete (groot, 8);
    fprintf(stdout, "tree after deleting 8: ");
    print_tree_inorder(groot);
    fprintf(stdout, "\n");

    groot = delete (groot, 55);
    fprintf(stdout, "tree after deleting 55: ");
    print_tree_inorder(groot);
    fprintf(stdout, "\n");

    return groot;
}

void* thread_func(void *arg) {
    thread_arg *targ = (thread_arg *)arg;

    pthread_mutex_lock(&(groot->lock));  // Acquire lock for root node

    fprintf(stdout, "Thread: count = %d\n", targ->count);

    for (int i = 0; i < targ->count; i++) {
        if (targ->op_start[i].type == SEARCH) {
            // Acquire lock for the node being searched
            node* result = search(groot, targ->op_start[i].val);
            // Release lock for the node being searched
        } else if (targ->op_start[i].type == INSERT) {
            // Acquire lock for the parent node where insertion will happen
            groot = insert(groot, targ->op_start[i].val);
            // Release lock for the parent node
        } else if (targ->op_start[i].type == DELETE) {
            // Acquire lock for the parent node where deletion will happen
            groot = delete(groot, targ->op_start[i].val);
            // Release lock for the parent node
        }
    }

    pthread_mutex_unlock(&(groot->lock));  // Release lock for root node

    return NULL;
}



int main (int argc, char* argv[]){

    // see RandGen
    srand(time(NULL));

    test_tree();
    // return 0;

    // initialize the thread structures
    op ops[NUMOPS];
    init_ops(ops);
    
    // just in case
    groot = NULL;

    pthread_t tids[NUMTHREADS];
    thread_arg targs[NUMTHREADS];
    // create therads
    for (int i=0; i<NUMTHREADS; i++) {
        targs[i].count = NUMOPS/NUMTHREADS; // assume perfect division
        targs[i].op_start = ops + i;
        pthread_create( &tids[i], NULL, thread_func, &targs[i]);
    }

    for (int i=0; i<NUMTHREADS; i++)
        pthread_join( tids[i], NULL);
    
    print_tree_inorder(groot);

    return 0;
}
