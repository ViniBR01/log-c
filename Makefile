CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC_DIR = src
TARGET = log_c.out
SRC = $(SRC_DIR)/log_c.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
