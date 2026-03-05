CC = gcc
CCFLAGS = -pedantic -Wall
EXEC = bash
TEST_EXECS = touch
LIB_DIR = src/commands
TEST_DIR = tests

LIB_SRC = $(wildcard $(LIB_DIR)/*.c) 
LIB_H = $(patsubst %.c,%.h,$(LIB_SRC))
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
SRC = $(LIB_SRC) src/main.c
OBJS = $(patsubst %.c,%.o,$(SRC))
LIB_OBJS = $(patsubst %.c,%.o,$(LIB_SRC))
TEST_OBJS = $(patsubst %.c,%.o,$(TEST_SRC))

all: $(EXEC)

tests: $(TEST_EXECS)

clean:
	rm -f $(OBJS) $(EXEC) $(TEST_EXECS)

$(EXEC): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIB_H)

$(TEST_EXECS): %: $(TEST_DIR)/%.o $(LIB_DIR)/%.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIB_DIR)/$@.h -lcunit

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
