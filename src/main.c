#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

// The memory
word memory[TOTAL_RAM];

char *in_buffer;

struct _stack {
    word value;
    struct _stack *next;
};
typedef struct _stack stack;
stack *STACK_HEAD = 0;

word PTR = 0;

enum state_codes {
    halt, //0
    set, //1
    push, //2
    pop, //3
    eq, //4
    gt, //5
    jmp, //6
    jt, //7
    jf, //8
    add, //9
    mult, //10
    mod, //11
    and, //12
    or, //13
    not, //14
    rmem, //15
    wmem, //16
    call, //17
    ret, //18
    out, //19
    in, //20
    noop //21
};

int main(int argc, char **argv)
{
    memset(memory, 0, sizeof(memory));

    if (argc != 2) {
        printf ("\nUsage: synacor-vm program.bin\n");
        exit(1);
    }

    FILE *program = fopen(argv[1], "rb");
    int i=0;
    word *mem = memory;
    while( !feof(program)) {
        fread(mem, sizeof(word), 1, program);
        ++mem;
#if DEBUG == 1
        printf("%s (%d)\n", "read your program into memory...", memory[i]);
#endif
        ++i;
    }

#if SHOW_BOOTSTRAP == 1
    printf("%s\n", "Your program is completely loaded into memory");
#endif

    run_program();

    return EXIT_SUCCESS;
}

void run_program(void)
{
    word command;
    while((command = memory[PTR]) != 0) {
        switch (command) {
            case halt:
                halt_state();
                break;
            case set:
                set_state();
                break;
            case push:
                push_state();
                break;
            case pop:
                pop_state();
                break;
            case eq:
                eq_state();
                break;
            case gt:
                gt_state();
                break;
            case jmp:
                jmp_state();
                break;
            case jt:
                jt_state();
                break;
            case jf:
                jf_state();
                break;
            case add:
                add_state();
                break;
            case mult:
                mult_state();
                break;
            case mod:
                mod_state();
                break;
            case and:
                and_state();
                break;
            case or:
                or_state();
                break;
            case not:
                not_state();
                break;
            case rmem:
                rmem_state();
                break;
            case wmem:
                wmem_state();
                break;
            case call:
                call_state();
                break;
            case ret:
                ret_state();
                break;
            case out:
                out_state();
                break;
            case in:
                in_state();
                break;
            case noop:
                noop_state();
                break;
            default:
                break;
        }
    }
}

word mem_resolve(word data)
{
	if (data >= RAM_END) {
		data = memory[data];
	}

	return data;
}

void halt_state(void)
{
    exit(halt);
}

void set_state(void)
{
	word reg = memory[PTR+1];
    memory[reg] = mem_resolve(memory[PTR+2]);

    PTR += 3;
}

void push_state(void)
{
    word value = mem_resolve(memory[PTR+1]);
    stack *elem = (stack *)malloc(sizeof(stack));
    elem->value = value;

    elem->next = STACK_HEAD;
    STACK_HEAD = elem;

    PTR += 2;
}

void pop_state(void)
{
    stack *elem = STACK_HEAD;

    if (elem == NULL) {
        printf("FATAL ERROR: EMPTY STACK!\n");
        exit(1);
    }

    STACK_HEAD = elem->next;
    word reg = memory[PTR+1];
	memory[reg] = elem->value;

    free(elem);

    PTR += 2;
}

void eq_state(void)
{
    word b = mem_resolve(memory[PTR+2]);
    word c = mem_resolve(memory[PTR+3]);

    word reg = memory[PTR+1];
    memory[reg] = 0;
    if (b == c) {
		memory[reg] = 1;
    }

    PTR += 4;
}

void gt_state(void)
{
    word b = mem_resolve(memory[PTR+2]);
    word c = mem_resolve(memory[PTR+3]);

    word reg = memory[PTR+1];
	memory[reg] = 0;
    if (b > c) {
        memory[reg] = 1;
    }

    PTR += 4;
}

void jmp_state(void)
{
    PTR = memory[PTR+1];
}

void jt_state(void)
{
    if (mem_resolve(memory[PTR+1]) != 0) {
        PTR = mem_resolve(memory[PTR+2]);
    } else {
    	PTR += 3;
    }
}

void jf_state(void)
{
    if (mem_resolve(memory[PTR+1]) == 0) {
        PTR = mem_resolve(memory[PTR+2]);
    } else {
        PTR += 3;
    }
}

void add_state(void)
{
	word reg = memory[PTR+1];
	memory[reg] = (mem_resolve(memory[PTR+2]) + mem_resolve(memory[PTR+3])) % MODULO;
    PTR += 4;
}

void mult_state(void)
{

	word reg = memory[PTR+1];
	memory[reg] = (mem_resolve(memory[PTR+2]) * mem_resolve(memory[PTR+3])) % MODULO;
    PTR += 4;
}

void mod_state(void)
{
	word reg = memory[PTR+1];
	memory[reg] = (mem_resolve(memory[PTR+2]) % mem_resolve(memory[PTR+3]));
    PTR += 4;
}

void and_state(void)
{
	word reg = memory[PTR+1];
	memory[reg] = mem_resolve(memory[PTR+2]) & mem_resolve(memory[PTR+3]);
    PTR += 4;
}

void or_state(void)
{
	word reg = memory[PTR+1];
	memory[reg] = mem_resolve(memory[PTR+2]) | mem_resolve(memory[PTR+3]);
    PTR += 4;
}

void not_state(void)
{
	word reg = memory[PTR+1];
	memory[reg] = mem_resolve(memory[PTR+2]) ^ 0x7FFF;
    PTR += 3;
}

void rmem_state(void)
{
	word a = memory[PTR+1];
	word b = memory[PTR+2];

    memory[a] = memory[mem_resolve(b)];
    PTR += 3;
}

void wmem_state(void)
{
	word a = memory[PTR+1];
	word b = memory[PTR+2];

	memory[mem_resolve(a)] = mem_resolve(b);

   	PTR += 3;
}

void call_state(void)
{
    stack *elem = (stack *)malloc(sizeof(stack));

    elem->value = PTR+2;
    elem->next = STACK_HEAD;
    STACK_HEAD = elem;

    PTR = mem_resolve(memory[PTR+1]);
}

void ret_state(void)
{
    if (STACK_HEAD == NULL) {
        printf("FATAL ERROR: EMPTY STACK!\n");
        exit(1);
    }

    stack *elem = STACK_HEAD;
    STACK_HEAD = elem->next;

    PTR = elem->value;
}

void out_state(void)
{
    printf("%c", mem_resolve(memory[PTR+1]));

    PTR += 2;
}

void in_state(void)
{
	    printf("SCANF: ");
    size_t n = 100;
    getline(&in_buffer, &n, stdin);

    word reg = memory[PTR+1];
    memory[reg] = (word)in_buffer[0];
    PTR += 2;
}

void noop_state(void)
{
    PTR += 1;
}

