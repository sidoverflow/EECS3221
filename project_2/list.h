/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node
{
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert_fcfs(struct node **head, Task *task);
void delete (struct node **head, Task *task);
void traverse(struct node *head);
