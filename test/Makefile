CC = gcc
UNITY_SRC = ../3rd-party/unity/src/unity.c
LIB = ../liblogc.a
C_FLAGS = -I../src -I../3rd-party/unity/src

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
