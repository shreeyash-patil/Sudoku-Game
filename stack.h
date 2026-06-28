#define N 9
#define EMPTY 0

typedef struct data{
    int num, row, col;
}data;

typedef struct stacknode{
    data d;
    struct stacknode *next;
}stacknode;

typedef struct stack{
    stacknode *top;
}stack;

void initstack(stack *s);
void init_undo_redo(stack *u, stack *r);
int stack_Empty(stack *s);
void push(stack *s, data prev_step);
data pop(stack *s);
data peek(stack *s);
void freestack(stack *s);
void free_undo_redo(stack *u, stack *r);