/**
 * Implementation of various scheduling algorithms.
 *
 * Round-robin scheduling with Priority
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;

// pointer to the struct containing the next task
struct node *tmp;

int cnt = 1;//counts how many processes have the same priority

Task *pickNextTask();
void insert_tail(struct node **head, Task *task);

// add a new task to the list of tasks
void add(char *name, int priority, int burst) {
    // first create the new task
    Task *newTask = (Task *) malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    // insert the new task into the list of tasks 
    insert(&head, newTask);
}

/**
 * Run the priority scheduler
 */
void schedule() 
{
    Task *current;
    //Task *previous;

    tmp = head;

    while (head != NULL) {
        current = pickNextTask();
        if (cnt > 1){
            //there are duplicate priorities, run as normal round robin
            //BUT make sure to insert at the tail
            //we only need to insert at the tail if the burst is larger than the quantum
            /*
            this allows for
            T5 for (QUANTUM) units
            T4 for (QUANTUM) units
            T5 for (QUANTUM) units

            */
            if (current->burst > QUANTUM) {
                
                run(current, QUANTUM);

                current->burst -= QUANTUM;
                delete(&head, current);
                insert_tail(&head, current);
            }
            else {
                run(current, current->burst);
            
                current->burst = 0;

                printf("Task %s finished.\n",current->name);        
                delete(&head, current);
            }
        }
        else{
            //no duplicate priorities, run as normal round robin
            run(current, current->burst);
            current->burst = 0;
            printf("Task %s finished.\n",current->name);        
            delete(&head, current);

        }
    }
}

/*
Puts the new node at the end of the list
*/
void insert_tail(struct node **head, Task *task){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = task;
    newNode->next = NULL;

    
    if ((*head) == NULL){
        *head = newNode;
        //the head is empty so we place the new node at the end of the list since head is the end
    }
    else{
        //not at the end of the list, keep looking for it by traversing the list
        struct node *tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        
        tmp->next = newNode;
    }
}

/**
 * Returns the next task selected to run.
 */
Task *pickNextTask()
{

    struct node *temp;
    Task *hp = head->task;
    temp = head->next;

    cnt = 1;//how many processes with the same priority
    while (temp != NULL){
        if (temp->task->priority > hp->priority ){
            hp = temp->task;
            cnt = 1;//not the same priority, and bigger so reset to 1
        }
        else if (temp->task->priority == hp->priority){
            cnt++;//same priority, increment
        }
        temp = temp->next;
    }

    return hp;

 
}

