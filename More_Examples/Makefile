CC     = mpicxx
CFLAGS = -O2

all: life-blocking life-nonblocking


life-blocking: life-blocking.C
	mpicxx -O2 -o life-blocking $<

life-nonblocking: life-nonblocking.C
	mpicxx -O2 -o life-nonblocking $<

clean:
	rm -f life-nonblocking life-blocking
	