CFLAGS = -Wall -fsanitize=address
CC = clang

all: main

test: all
	./main

main: main.o libmat.a
	$(CC) $(CFLAGS) $^ -o $@

libmat.a: matrix.o matrix.h
	ar rcs $@ $(filter %.o,$^)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f matrix.o main main.o libmat.a
