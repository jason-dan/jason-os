/****************************************************
 * readyqueue.c - This is the home of the kernel's  *
 * ready queue. Contains all underlying             *
 * functionality as well as public interfaces.      *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include "readyqueue.h"
#include "pcb.h"

typedef struct queue_node_t{
    PCB *pcb;
    struct queue_node_t *next;
} QUEUE_NODE_T;

typedef struct {
    QUEUE_NODE_T *head;
    QUEUE_NODE_T *tail;
    int size;
} READY_QUEUE_T;

READY_QUEUE_T* READY_QUEUE = NULL;

READY_QUEUE_T* initReadyQueue();
QUEUE_NODE_T* newQueueNode(PCB *pcb);
PCB* getNodePCB(QUEUE_NODE_T* node);

PCB* ReadyQueue__pop() {
    if (READY_QUEUE == NULL) READY_QUEUE = initReadyQueue();

    if (READY_QUEUE->size == 0) {
        return NULL;
    }

    QUEUE_NODE_T* oldHead = READY_QUEUE->head;
    READY_QUEUE->head = oldHead->next;
    READY_QUEUE->size--;

    return getNodePCB(oldHead);
};

void ReadyQueue__push(PCB* pcb) {
    if (READY_QUEUE == NULL) READY_QUEUE = initReadyQueue();

    QUEUE_NODE_T* newNode = newQueueNode(pcb);

    if (READY_QUEUE->size == 0) {
        READY_QUEUE->head = newNode;
        READY_QUEUE->tail = newNode;
    } else {
        QUEUE_NODE_T *oldTail = READY_QUEUE->tail;
        oldTail->next = newNode;
        READY_QUEUE->tail = newNode;
    }
    
    READY_QUEUE->size++;
}

void ReadyQueue__cleanup() {
    if (READY_QUEUE == NULL) return;
    
    while (READY_QUEUE->size > 0) {
        PCB* pcb = ReadyQueue__pop();
        free(pcb);
    }

    free(READY_QUEUE);
    READY_QUEUE = NULL;

}
READY_QUEUE_T* initReadyQueue() {
    READY_QUEUE_T *newQueue = malloc(sizeof(READY_QUEUE_T));
    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue->size = 0;
    return newQueue;
}
QUEUE_NODE_T* newQueueNode(PCB *pcb) {
    QUEUE_NODE_T* newNode = malloc(sizeof(QUEUE_NODE_T));
    newNode->pcb = pcb;
    newNode->next = NULL;
    return newNode;
}

PCB* getNodePCB(QUEUE_NODE_T* node) {
    PCB* pcb = node->pcb;
    free(node);
    return pcb;
}