CC = gcc
UNITY_SRC = ../3rd-party/unity/src/unity.c
LIB = ../liblogc.a
CFLAGS = -I../src -I../3rd-party/unity/src

.PHONY: all run clean

# 'all' builds the test binaries but does not run them. Use 'run' to execute.
all: TestLogC.out TestBackendInjection.out

run: all
	./TestLogC.out
	./TestBackendInjection.out

TestLogC.out: TestLogC.c $(UNITY_SRC) $(LIB)
	$(CC) $(CFLAGS) TestLogC.c $(UNITY_SRC) $(LIB) -o $@

TestBackendInjection.out: TestBackendInjection.c $(UNITY_SRC) $(LIB)
	$(CC) $(CFLAGS) TestBackendInjection.c $(UNITY_SRC) $(LIB) -o $@

clean:
	rm -f *.out *.o TestLogC TestBackendInjection
