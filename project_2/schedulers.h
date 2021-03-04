#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list
void add(char *name, int priority, int burst);

// invoke the scheduler
void schedule();

// merge sort functions
void sort(struct node **root);
void split(struct node *root, struct node **left, struct node **right);
struct node *merge(struct node *left, struct node *right);