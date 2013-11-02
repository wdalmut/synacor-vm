#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"

void mem_boot_from_file(FILE *);

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf ("\nUsage: synacor-vm <program.bin>\n\n");
        exit(1);
    }

    mem_reset();

    FILE *program = fopen(argv[1], "rb");
    mem_boot_from_file(program);
    fclose(program);

    run_program();

    return EXIT_SUCCESS;
}

void mem_boot_from_file(FILE *program)
{
    int i=0; word mem;
    while( !feof(program)) {
        fread(&mem, sizeof(word), 1, program);
        mem_write(i++, mem);
    }
}



