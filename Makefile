CC = gcc
CCFLAGS = -pedantic -Wall -g
EXEC = bash
TEST_EXECS = touch_test
LIB_DIR = src/commands/*
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

touch_test: $(TEST_DIR)/touch.o $(LIB_DIR)/touch.o $(LIB_DIR)/ls.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIB_DIR)/touch.h $(LIB_DIR)/ls.h -lcunit

ls_test: $(TEST_DIR)/ls.o $(LIB_DIR)/ls.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIB_DIR)/ls.h -lcunit

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
