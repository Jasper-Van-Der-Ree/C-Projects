#include <stdlib.h>
#include <stdio.h>

// A program to perform a LEVEL ORDER (BREADTH-FIRST) TRAVERSAL of a binary search tree

//STRUCTURES
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;
typedef struct BSTNode BSTNode;

struct QueueNode {
    BSTNode* data;
    struct QueueNode* next; // pointer to next node in linked list
};
struct Queue {
    struct QueueNode* front; // front (head) of the queue
    struct QueueNode* back; // back (tail) of the queue
};
struct BSTNode {
    int val;
    int explored;
    struct BSTNode* l_child; // nodes with smaller key will be in left subtree
    struct BSTNode* r_child; // nodes with larger key will be in right subtree
};

//QUEUE
void enqueue (Queue* queue, BSTNode* data ) {
    QueueNode* queueNode = malloc(sizeof(QueueNode));
    queueNode -> data = data;
    queueNode -> next = NULL; // At back of the queue, there is no next node.

    if (queue->back==NULL) { // If the Queue is currently empty
        queue->front = queueNode;
        queue->back = queueNode;
    } else {
        queue->back->next = queueNode;
        queue->back = queueNode;
    }

    return;
}

unsigned char dequeue ( Queue* queue ) {

    if (queue->front==NULL) { // If the Queue is currently empty
        return '\0';
    } else {

        // The QueueNode at front of the queue to be removed
        QueueNode* temp = queue->front;
        unsigned char data = temp->data->val;

        queue->front = temp->next;
        if (queue->back==temp) { // If the Queue will become empty
            queue->back = NULL;
        }

        free(temp);
        return data;
    }
}

//BST
BSTNode* insert(BSTNode* root, unsigned int val){
    if(root == NULL){
        root = malloc(sizeof(BSTNode));
        root->l_child = NULL;
        root->r_child = NULL;
        root->val = val;
    }
    else if(val < root->val){
        root->l_child = insert(root->l_child, val);
    }
    else if(val == root->val){
        return root;
    }
    else{
        root->r_child = insert(root->r_child,val);
    }

    return root;
}

void delete_bst ( BSTNode* root ) {
    if ( root->r_child!=NULL ) {
        delete_bst (root->r_child);
    }
    if ( root->l_child!=NULL ) {
        delete_bst (root->l_child);
    }
    free (root);
}

//MAIN
int main ( int argc, char* argv[] ) {

    // READ INPUT FILE TO CREATE BINARY SEARCH TREE
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    BSTNode* root = NULL;
    int key;
    while ( fscanf(fp, "%d", &key)!=EOF ) {
        root = insert (root, key);
        // printf("%d ", key);
    }
    fclose(fp);

    // USE A QUEUE TO PERFORM LEVEL ORDER TRAVERSAL
    Queue queue = { .front=NULL, .back=NULL };
    BSTNode* currBSTNode;
    enqueue(&queue, root);

    while (queue.front){

        //assign currBSTNode
        currBSTNode = queue.front->data;
        currBSTNode->explored = 1;

        //Dequeue & Print
        dequeue(&queue);
        printf("%d ", currBSTNode->val);

        //Add Left and then Right Child
        if(currBSTNode->l_child && currBSTNode->l_child->explored){
            enqueue(&queue, currBSTNode->l_child);
        }
        if(currBSTNode->r_child && currBSTNode->r_child->explored){
            enqueue(&queue, currBSTNode->r_child);
        }

    }

    delete_bst(root);
    return EXIT_SUCCESS;
}
