CC = gcc
CCFLAGS = -pedantic -Wall
EXEC = bash
LIB_DIR = src/commands

LIB_SRC = $(wildcard $(LIB_DIR)/*.c) 
LIB_H = $(patsubst %.c,%.h,$(LIB_SRC))
SRC = $(LIB_SRC) src/main.c
OBJS = $(patsubst %.c,%.o,$(SRC))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIB_H)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
