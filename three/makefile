SRCONE = first.c
SRCTWO = second.c
FLAGS = -Wall -o

one:
	gcc ${SRCONE} ${FLAGS} first -pthread

two:
	gcc ${SRCTWO} ${FLAGS} second -pthread

clean:
	rm -f first second
