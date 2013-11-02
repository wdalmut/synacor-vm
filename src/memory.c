#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

// The memory
word memory[TOTAL_RAM];

stack *STACK_HEAD = 0;

void mem_reset(void)
{
    memset(memory, 0, sizeof(memory));
}

word mem_read(word addr)
{
    return memory[(int)addr];
}

void mem_write(int addr, word value)
{
    memory[addr] = value;
}

word mem_arg(word *ptr)
{
    return memory[(*ptr)++];
}

word mem_resolve(word data)
{
    if (data >= RAM_END) {
        data = memory[data];
    }

    return data;
}

void mem_stack_push(word value)
{
    stack *elem = (stack *)malloc(sizeof(stack));
    elem->value = value;

    elem->next = STACK_HEAD;
    STACK_HEAD = elem;
}

word mem_stack_pop(void)
{
    stack *elem = STACK_HEAD;

    if (elem == NULL) {
        printf("FATAL ERROR: EMPTY STACK!\n");
        exit(1);
    }

    STACK_HEAD = elem->next;
    word value = elem->value;
    free(elem);

    return value;
}
