#ifndef __MEMORY_H__
#define __MEMORY_H__

#define RAM_END 32768
#define TOTAL_RAM (RAM_END+8)

typedef unsigned short int word;

struct _stack {
    word value;
    struct _stack *next;
};
typedef struct _stack stack;

void mem_reset(void);
word mem_resolve(word);
word mem_arg(word *ptr);

word mem_read(word);
void mem_write(int, word);

void mem_stack_push(word);
word mem_stack_pop();

#endif
