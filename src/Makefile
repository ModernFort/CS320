CC = gcc
CCFLAGS = -pedantic -Wall
EXEC = bash
TEST_EXEC = touch
SUBFOLDER = commands
SRC_SUBFOLDER = src
TEST_SUBFOLDER = ../tests

LIB_SRC = $(wildcard $(SUBFOLDER)/*.c) 
SRC = $(LIB_SRC) main.c
TEST_SRC = $(wildcard $(TEST_SUBFOLDER)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRC))
TEST_OBJS = $(patsubst %.c,%.o,$(TEST_SRC))
LIB_OBJS = $(patsubst %.c,%.o,$(LIB_SRC))

all: ${EXEC}

${EXEC}: ${OBJS}
	${CC} -o $@ $^

%.o: %.c
	${CC} ${CCFLAGS} -c $< -o $@

# tests: $(TEST_OBJS)

test: $(TEST_OBJS) $(LIB_OBJS)
	${CC} $(CCFLAGS) -o run_tests $^ -lcunit

clean:
	rm -f ${OBJS} ${EXEC} ${TEST_OBJS}
