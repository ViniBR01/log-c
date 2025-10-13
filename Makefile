CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC_DIR = src
LIB_SRC = $(SRC_DIR)/log_c.c
LIB_OBJ = $(LIB_SRC:.c=.o)
LIB = liblogc.a

all: $(LIB)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

clean:
	rm -f $(LIB) $(LIB_OBJ)

.PHONY: all clean
