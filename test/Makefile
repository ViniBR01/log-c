CC = gcc
UNITY_SRC = unity/src/unity.c
TEST_SRC = TestLogC.c
TEST_TARGET = TestLogC.out
C_FLAGS = -I../src -I./unity/src

.PHONY: all clean

all: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(UNITY_SRC)
	$(CC) $(C_FLAGS) $^ -o $@

clean:
	rm -f $(TEST_TARGET) *.o
