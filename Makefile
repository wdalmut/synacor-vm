MKDIR=/bin/mkdir
RM=/bin/rm

EXEC=synacor-vm

CC=/usr/bin/gcc
LD=/usr/bin/ld
CFLAGS=-g3 -Wall

all: build

build: clean
	${MKDIR} -p build
	${CC} ${CFLAGS} -c -I src -o build/main.o src/main.c
	${CC} ${CFLAGS} -c -I src -o build/memory.o src/memory.c
	${CC} ${CFLAGS} -I src -o ${EXEC} build/main.o build/memory.o

clean:
	${RM} -rf bin
	${RM} -rf build
