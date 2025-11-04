CC         := gcc
CFLAGS     := -Wall -Wextra -O2

SRC_DIR    := src
LIB_SRC    := $(SRC_DIR)/log_c.c
LIB_OBJ    := $(LIB_SRC:.c=.o)
LIB        := liblogc.a

C_INCLUDES := -I$(SRC_DIR)

TEST_DIR   := test

.PHONY: all test clean

all: $(LIB)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

test: $(LIB)
	$(MAKE) -C $(TEST_DIR) run

clean:
	rm -f $(LIB) $(LIB_OBJ)
	-$(MAKE) -C $(TEST_DIR) clean
