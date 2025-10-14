CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC_DIR = src
PRINTF_DIR = 3rd-party/printf
LIB_SRC = $(SRC_DIR)/log_c.c
PRINTF_SRC = $(PRINTF_DIR)/printf.c
LIB_OBJ = $(LIB_SRC:.c=.o)
PRINTF_OBJ = $(PRINTF_SRC:.c=.o)
LIB = liblogc.a

all: $(LIB)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) -I$(PRINTF_DIR) -c -o $@ $<

$(PRINTF_OBJ): $(PRINTF_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(LIB_OBJ) $(PRINTF_OBJ)
	ar rcs $@ $^

clean:
	rm -f $(LIB) $(LIB_OBJ) $(PRINTF_OBJ)

.PHONY: all clean
