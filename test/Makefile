CC = gcc
UNITY_SRC = unity/src/unity.c
LIB = ../liblogc.a
C_FLAGS = -I../src -I./unity/src

.PHONY: all clean

all: TestLogC.out TestBackendInjection.out
	./TestLogC.out
	./TestBackendInjection.out

TestLogC.out: TestLogC.c $(UNITY_SRC) $(LIB)
	$(CC) $(C_FLAGS) TestLogC.c $(UNITY_SRC) $(LIB) -o $@

TestBackendInjection.out: TestBackendInjection.c $(UNITY_SRC) $(LIB)
	$(CC) $(C_FLAGS) TestBackendInjection.c $(UNITY_SRC) $(LIB) -o $@

clean:
	rm -f *.out *.o TestLogC TestBackendInjection
