CC = gcc
UNITY_SRC = unity/src/unity.c
SRCS = ../src/log_c.c
C_FLAGS = -I../src -I./unity/src -DLOGC_NO_MAIN

.PHONY: all clean

all: TestLogC.out TestBackendInjection.out
	./TestLogC.out
	./TestBackendInjection.out

TestLogC.out: TestLogC.c $(UNITY_SRC) $(SRCS)
	$(CC) $(C_FLAGS) $^ -o $@

TestBackendInjection.out: TestBackendInjection.c $(UNITY_SRC) $(SRCS)
	$(CC) $(C_FLAGS) $^ -o $@

clean:
	rm -f *.out *.o TestLogC TestBackendInjection
