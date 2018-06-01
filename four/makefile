SRCONE = haircut.c
SRCTWO = cig.c
FLAGS =  -o

one:
	gcc ${SRCONE} ${FLAGS} hair -pthread

two:
	gcc ${SRCTWO} ${FLAGS} cig -pthread


all:
	make one
	make two

clean:
	rm -f hair cig
