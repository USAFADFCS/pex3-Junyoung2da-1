/** pagequeue.c
 * ===========================================================
 * Name: C1C Andrew J. Kim, 23 April 2026
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    PageQueue* newQueue = NULL;
    //dynamically allocate memory
    newQueue = (PageQueue*)malloc(sizeof(PageQueue));
    if (newQueue == NULL){
        printf("something is wrong with the pqInit funciton, go fix it stupid\n");
        return NULL;
    }
    // size to 0, maxSize to maxSize, and return the pointer
    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue->maxSize = maxSize;
    newQueue->size = 0;
    return newQueue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 * @param pq pointer to the PageQueue
 * @param pageNum the page number being accessed
 * @return depth from MRU end (0 = most recent) if page was found,
 *         or -1 if the page was not in the queue (page fault)
 * @note On a hit, the page is moved to the MRU (tail) position.
 *       New pages are always inserted at the tail. If inserting
 *       causes size to exceed maxSize, the head (LRU) page
 *       is evicted.
 */
/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //check if the queue exists
    if (pq == NULL){
        printf("list address null for pqAccess function. FIX IT\n");
        return -1;
    }
    //set up node pointers for traversing list
    PqNode* currentNode = pq->tail;
    unsigned long tgtNum = pageNum;
    long tgtDepth = 0;
    long foundTgt = -1;

    // check if the page is resident in queue
    while (currentNode != NULL){
        //check if numbers match
        if (currentNode->pageNum == tgtNum){
            foundTgt = tgtDepth;
            break;
        }
        //increment location in queue for tracking purposes
        ++tgtDepth;
        
        //move onto the next node
        currentNode = currentNode->prev;
    }
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    if (foundTgt != -1){
        //if the page is already at the end of queue
        if (foundTgt == 0){
            return 0;
        }
        //if there's only one element
        else if (pq->size == 1){
            return 0;
        }
        //if the node is the head node
        else if (tgtDepth == (pq->size - 1)){
            //set new head and adjust its pointers
            pq->head = currentNode->next;
            pq->head->prev = NULL;
            
            //adjust pointers out of currentNode
            currentNode->prev = pq->tail;
            currentNode->next = NULL;

            // append current node to end of queue
            pq->tail->next = currentNode;
            pq->tail = currentNode;
        }
        else{
            // adjust pointers so currentnode is taken out of queue
            currentNode->prev->next = currentNode->next;
            currentNode->next->prev = currentNode->prev;

            //adjust currentNode's pointers
            currentNode->prev = pq->tail;
            currentNode->next = NULL;

            // jam currentNode at end of queue
            pq->tail->next = currentNode;
            pq->tail = currentNode;
        }
        return foundTgt;

    }
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    else{
        //allocate node and add it to tail
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        newNode->pageNum = pageNum;
        //set the tail to newnode and adjust pointers
        newNode->next = NULL;
        newNode->prev = pq->tail;

        //if the queue was empty from teh start
        if (pq->tail == NULL){
            pq->head = newNode;
        }
        else{
            pq->tail->next = newNode;
        }
        //set tail of queue to newNode
        pq->tail = newNode;
        pq->size++; // increment queue size


        //if queue is too big, free node at the head
        if (pq->size > pq->maxSize){ // take out the head node
            PqNode* tempNode = pq->head; 
            pq->head = pq->head->next;      //set head to next node
            //if queue isn't empty, set prev to null
            if (pq->head != NULL){
                pq->head->prev = NULL;          //set new head prev to NULL
            }
            else{
                pq->tail = NULL; 
            }
            free(tempNode);                 //deete previous head node
            pq->size--; //decrement queue size
        }
        
    }
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    //don't do anything if the list is already empty
    if (pq == NULL){
        return;
    }

    //set up node pointers for traversing list
    PqNode* current = pq->head;
    PqNode* nextNode = NULL;

    //go through the linked list and delete all nodes
    while (current != NULL){
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    //clear data from linked list structure
    pq->head = NULL;
    pq->maxSize = 0;
    pq->size = 0;
    pq->tail = NULL;
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    // do nothing if the list is null
    if (pq == NULL){
        printf("list is null, so it can't print\n");
        return;
    }
    // do nothing if the list is empty
    if (pq->head == NULL){
        printf("the list is empty\n");
        return;
    }
    if (pq->size == 0){
        printf("list is empty\n");
        return;
    }

    // go through the list and print the values in the nodes
    PqNode* current = pq->head;
    while (current != NULL){
        printf("%lu\n", current->pageNum);
        current = current->next;
        
    }
    printf("NULL\n");
}

