CFLAGS = -Wall -fsanitize=address -g
CC = clang

all: main

test: all
	./main

main: main.o libmat.a libnet.a
	$(CC) $(CFLAGS) $^ -o $@

libmat.a: matrix.o matrix.h
	ar rcs $@ $(filter %.o,$^)

libnet.a: neural_net.o neural_net.h
	ar rcs $@ $(filter %.o,$^)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c $<

neural_net.o: neural_net.c neural_net.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f matrix.o neural_net.o main main.o libmat.a libnet.a
