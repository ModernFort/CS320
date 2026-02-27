CC = gcc
CCFLAGS = -pedantic -Wall
EXEC = bash
SUBFOLDER = commands
LIB_SRC = $(wildcard $(SUBFOLDER)/*.c) 
SRC = $(LIB_SRC) main.c
# OBJS = $(SRC:.c=.o)
OBJS = $(patsubst %.c,%.o,$(SRC))

all: ${EXEC}
	@echo "LIBSRC: $(LIB_SRC)"
	@echo "OBJS: $(OBJS)"

${EXEC}: ${OBJS}
	${CC} -o $@ $^

%.o: %.c %.h
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS} ${EXEC}
