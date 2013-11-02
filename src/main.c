#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "memory.h"

char *in_buffer;

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
    mem_reset();

    if (argc != 2) {
        printf ("\nUsage: synacor-vm <program.bin>\n\n");
        exit(1);
    }

    FILE *program = fopen(argv[1], "rb");
    int i=0; word mem;
    while( !feof(program)) {
        fread(&mem, sizeof(word), 1, program);
        mem_write(i++, mem);
#if DEBUG == 1
        printf("%s (%d)\n", "read your program into memory...", memory[i]);
#endif
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
    while(1 == 1) {
    	command = mem_arg(&PTR);
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

void halt_state(void)
{
    exit(halt);
}

void set_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_resolve(mem_arg(&PTR));

	mem_write(a, b);
}

void push_state(void)
{
	word a = mem_resolve(mem_arg(&PTR));
    mem_stack_push(a);
}

void pop_state(void)
{
	word a = mem_arg(&PTR);
    word value = mem_stack_pop();

    mem_write(a, value);
}

void eq_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_resolve(mem_arg(&PTR));
	word c = mem_resolve(mem_arg(&PTR));

	mem_write(a, 0);
    if (b == c) {
		mem_write(a, 1);
    }
}

void gt_state(void)
{
	word a = mem_arg(&PTR);
    word b = mem_resolve(mem_arg(&PTR));
    word c = mem_resolve(mem_arg(&PTR));

    mem_write(a, 0);
    if (b > c) {
        mem_write(a, 1);
    }
}

void jmp_state(void)
{
    PTR = mem_arg(&PTR);
}

void jt_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
    if (mem_resolve(a) != 0) {
        PTR = mem_resolve(b);
    }
}

void jf_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
    if (mem_resolve(a) == 0) {
        PTR = mem_resolve(b);
    }
}

void add_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	word c = mem_arg(&PTR);
	mem_write(a, (mem_resolve(b) + mem_resolve(c)) % MODULO);
}

void mult_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	word c = mem_arg(&PTR);
	mem_write(a, (mem_resolve(b) * mem_resolve(c)) % MODULO);
}

void mod_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	word c = mem_arg(&PTR);
	mem_write(a, (mem_resolve(b) % mem_resolve(c)));
}

void and_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	word c = mem_arg(&PTR);
	mem_write(a, mem_resolve(b) & mem_resolve(c));
}

void or_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	word c = mem_arg(&PTR);
	mem_write(a, mem_resolve(b) | mem_resolve(c));
}

void not_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);
	mem_write(a, mem_resolve(b) ^ 0x7FFF);
}

void rmem_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);

    mem_write(a, mem_read(mem_resolve(b)));
}

void wmem_state(void)
{
	word a = mem_arg(&PTR);
	word b = mem_arg(&PTR);

	mem_write(mem_resolve(a), mem_resolve(b));
}

void call_state(void)
{
	word a = mem_resolve(mem_arg(&PTR));

	mem_stack_push(PTR);
	PTR = a;
}

void ret_state(void)
{
    PTR = mem_stack_pop();
}

void out_state(void)
{
	word a = mem_arg(&PTR);
    printf("%c", mem_resolve(a));
}

void in_state(void)
{
	char value = getchar();

    word a = mem_arg(&PTR);
    mem_write(a, value);
}

void noop_state(void)
{

}

