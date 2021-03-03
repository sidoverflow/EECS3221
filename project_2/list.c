/**
 * Various list operations
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"

// add a new task to the list of tasks
void insert_fcfs(struct node **head, Task *newTask)
{
    struct node *temp;
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = NULL;
    if (*head == NULL)
    {
        *head = newNode; //when linked list is empty
    }
    else
    {
        temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next; //traverse the list until p is the last node.The last node always points to NULL.
        }
        temp->next = newNode; //Point the previous last node to the new node created.
    }
}

// delete the selected task from the list
void delete (struct node **head, Task *task)
{
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name, temp->task->name) == 0)
    {
        *head = (*head)->next;
    }
    else
    {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name, temp->task->name) != 0)
        {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
void traverse(struct node *head)
{
    struct node *temp;
    temp = head;

    while (temp != NULL)
    {
        printf("[%s] [%d] [%d]\n", temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}