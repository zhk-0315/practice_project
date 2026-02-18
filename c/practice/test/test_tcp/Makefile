CC = /usr/bin/gcc
MAKE = /usr/bin/make

RECV = recv
SEND = send
TARGET = ${RECV} ${SEND}

#$(error "$${RECV}=${RECV} $${SEND}=${SEND}")

.PHONY: all clean
all: ${TARGET} 
	@echo "make all"

${RECV}: ${RECV}.c
	@echo "make ${RECV}"
	gcc -o ${RECV}.c.o -c $^
	gcc -o $@  ${RECV}.c.o

${SEND}: ${SEND}.c
	@echo "make ${SEND}"
	gcc -o ${SEND}.c.o -c $^
	gcc -o $@  ${SEND}.c.o

clean:
	@echo "make clean"
	@rm -f ${TARGET} ${RECV}.c.o ${SEND}.c.o
